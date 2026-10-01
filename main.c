#include <stdio.h> 

// Definição de constantes do sistema
#define MAX_ENCOMENDAS 100
#define META_FATURAMENTO 1000.00

// ==========================================
// VARIÁVEIS GLOBAIS
// Acumulam dados ao longo da execução do programa
// ==========================================
int total_encomendas = 0;   // Contador de encomendas registradas
float faturamento_total = 0.0; // Soma do valor total arrecadado
float soma_peso = 0.0;        // Soma dos pesos para calcular a média
float soma_distancia = 0.0;   // Soma das distâncias para calcular a média

float maior_frete = 0.0; // Guarda o maior valor de frete calculado
float menor_frete = 0.0; // Guarda o menor valor de frete calculado

int qtd_padrao = 0;      // Quantidade de encomendas da categoria Padrão
int qtd_expressa = 0;     // Quantidade de encomendas da categoria Expressa
int qtd_prioritaria = 0;  // Quantidade de encomendas da categoria Prioritária
int qtd_isencoes = 0;     // Quantidade de encomendas com frete grátis

// ==========================================
// PROTÓTIPOS DAS FUNÇÕES
// Declaração antecipada das funções do código
// ==========================================
void programa();
void cadastro();
void registro();
void indicadores();

int solicitar_cupom();
float calcular_taxa_peso(float peso);
int verificar_frete_gratis(int categoria, float peso, float distancia);
void processar_encomenda(int categoria, float peso, float distancia, int cupom);

// Função principal de entrada da execução
int main(int argc, char **argv)
{
    programa(); // Inicia o menu principal do programa
    return 0;   // Retorna 0 indicando execução com sucesso
}

// ==========================================
// MENU PRINCIPAL DO SISTEMA
// Usa um loop infinito para manter o sistema ativo
// ==========================================
void programa()
{
    int opcao; // Guarda a opção escolhida pelo usuário

    for (;;) // Loop infinito para reexibir o menu até o usuário escolher sair (opção 4)
    {
        printf("======================================\n");
        printf("LOGTECH LOGISTICS MENU\n");
        printf("======================================\n");
        printf("1-Cadastrar e Processar encomenda\n");
        printf("2-Exibir Resumo Estatistico do dia\n");
        printf("3-Exibir Indicadores\n");
        printf("4-Sair\n");
        printf("Escolha uma opcao: ");
        
        // Se a leitura do scanf falhar (usuário digitou texto em vez de número)
        if (scanf("%d", &opcao) != 1) {
            printf("\nEntrada invalida!\n\n");
            while (getchar() != '\n'); // Limpa o buffer de entrada do teclado
            continue; // Volta para o início do loop
        }

        // Seleção de ações com base na opção digitada
        switch (opcao)
        {
        case 1:
            // Verifica se o limite máximo de encomendas diárias foi atingido
            if (total_encomendas >= MAX_ENCOMENDAS) {
                printf("\nLimite maximo de 100 encomendas atingido para hoje!\n\n");
            } else {
                cadastro(); // Chama a função para cadastrar pacote
            }
            break;

        case 2:
            registro(); // Exibe estatísticas gerais
            break;

        case 3:
            indicadores(); // Exibe metas e contadores operacionais
            break;

        case 4:
            printf("\nSaindo do Sistema...\n");
            return; // Encerra a função programa() e finaliza o sistema

        default:
            printf("\nOpcao Invalida!\n\n"); // Caso o número não seja de 1 a 4
            break;
        }
    }
}

// ==========================================
// CADASTRO E VALIDAÇÃO DE DADOS
// Lê as informações do pacote e garante dados válidos
// ==========================================
void cadastro()
{
    int categoria = 0;
    float peso = 0.0, distancia = 0.0;
    char tem_cupom = 'N';
    int codigo_cupom = 0;

    printf("\n======================================\n CADASTRAR PACOTE \n======================================\n");

    // Loop do-while para garantir que a categoria digitada seja válida (1, 2 ou 3)
    do {
        printf("Qual a Categoria do Pacote? (1- Padrao, 2- Expresso, 3- Prioritario): ");
        if (scanf("%d", &categoria) != 1) {
            while (getchar() != '\n'); // Limpa entrada em caso de digitação inválida
            categoria = 0;
        }
        if (categoria < 1 || categoria > 3) {
            printf("Categoria invalida! Escolha 1, 2 ou 3.\n");
        }
    } while (categoria < 1 || categoria > 3);

    // Loop do-while para garantir que o peso seja maior que zero
    do {
        printf("Qual o peso do pacote (EM KG): ");
        if (scanf("%f", &peso) != 1) {
            while (getchar() != '\n');
            peso = 0.0;
        }
        if (peso <= 0) {
            printf("O peso deve ser estritamente maior que 0 kg!\n");
        }
    } while (peso <= 0);

    // Loop do-while para garantir que a distância seja maior que zero
    do {
        printf("Me diga a distancia do pedido (Em KM): ");
        if (scanf("%f", &distancia) != 1) {
            while (getchar() != '\n');
            distancia = 0.0;
        }
        if (distancia <= 0) {
            printf("A distancia deve ser estritamente maior que 0 km!\n");
        }
    } while (distancia <= 0);

    // Loop para validação da resposta de cupom ('S', 's', 'N' ou 'n')
    do {
        printf("Teve cupom? (S/N): ");
        scanf(" %c", &tem_cupom);
        if (tem_cupom != 'S' && tem_cupom != 's' && tem_cupom != 'N' && tem_cupom != 'n') {
            printf("Opcao invalida! Digite S para Sim ou N para Nao.\n");
        }
    } while (tem_cupom != 'S' && tem_cupom != 's' && tem_cupom != 'N' && tem_cupom != 'n');

    // Se o usuário confirmou que possui cupom, chama a função de leitura
    if (tem_cupom == 'S' || tem_cupom == 's') {
        codigo_cupom = solicitar_cupom();
    }

    // Processa o cálculo do frete com os dados coletados
    processar_encomenda(categoria, peso, distancia, codigo_cupom);
}

// ==========================================
// SOLICITAÇÃO E VALIDAÇÃO DE CUPOM
// ==========================================
int solicitar_cupom()
{
    int cupom = 0;
    while (1) { // Loop executado até o usuário inserir um cupom válido ou cancelar
        printf("Qual o codigo do cupom (1020 ou 2030, ou 0 para cancelar): ");
        if (scanf("%d", &cupom) != 1) {
            while (getchar() != '\n');
            cupom = -1;
        }

        // Verifica se o cupom inserido é um dos aceitos pelo sistema
        if (cupom == 1020 || cupom == 2030 || cupom == 0) {
            return cupom; // Retorna o código válido e encerra a função
        } else {
            printf("Codigo de cupom invalido! Tente novamente.\n");
        }
    }
}

// ==========================================
// CÁLCULO DA TAXA BASE POR PESO
// ==========================================
float calcular_taxa_peso(float peso)
{
    if (peso <= 5.0) {
        return peso * 2.00; // Pacotes até 5kg custam R$ 2,00 por kg
    } else {
        return peso * 3.50; // Pacotes com mais de 5kg custam R$ 3,50 por kg
    }
}

// ==========================================
// REGRA DE ISENÇÃO (FRETE GRÁTIS)
// Retorna 1 para verdadeiro e 0 para falso
// ==========================================
int verificar_frete_gratis(int categoria, float peso, float distancia)
{
    // Regra: Categoria Padrão (1) + até 2kg + até 10km recebe frete grátis
    if (categoria == 1 && peso <= 2.0 && distancia <= 10.0) {
        return 1; // Isento
    } else {
        return 0; // Não isento
    }
}

// ==========================================
// PROCESSAMENTO DA ENCOMENDA E IMPRESSÃO
// Realiza os cálculos do frete e atualiza as globais
// ==========================================
void processar_encomenda(int categoria, float peso, float distancia, int cupom)
{
    float multiplicador_dist = 0.0;
    float taxa_adicional = 0.0;

    // Define os parâmetros com base na categoria escolhida
    switch (categoria) {
    case 1:
        multiplicador_dist = 1.20;
        taxa_adicional = 0.0;
        qtd_padrao++; // Incrementa contador da categoria Padrão
        break;
    case 2:
        multiplicador_dist = 1.80;
        taxa_adicional = 15.00;
        qtd_expressa++; // Incrementa contador da categoria Expressa
        break;
    case 3:
        multiplicador_dist = 2.50;
        taxa_adicional = 30.00;
        qtd_prioritaria++; // Incrementa contador da categoria Prioritária
        break;
    }

    // Cálculo das taxas brutas
    float valor_peso = calcular_taxa_peso(peso);
    float frete_bruto = (distancia * multiplicador_dist) + valor_peso + taxa_adicional;
    float valor_final = frete_bruto;

    // Checa se a encomenda se enquadra na isenção
    int frete_gratis = verificar_frete_gratis(categoria, peso, distancia);

    if (frete_gratis == 1) {
        valor_final = 0.0; // Zerado por frete grátis
        qtd_isencoes++;    // Incrementa contador de isenções
    } else {
        // Desconto de 10% no valor bruto para distâncias maiores que 100km
        if (distancia > 100.0) {
            valor_final -= (frete_bruto * 0.10);
        }
        // Aplicação dos descontos por cupom
        if (cupom == 1020) { // Cupom LOG10: 10% de desconto acumulado
            valor_final -= (valor_final * 0.10);
        } else if (cupom == 2030) { // Cupom DESCONTO15: R$ 15,00 de desconto
            valor_final -= 15.00;
            if (valor_final < 0.0) valor_final = 0.0; // Garante que o valor não fique negativo
        }
    }

    // Atualização dos totais operacionais acumulados
    total_encomendas++;
    soma_peso += peso;
    soma_distancia += distancia;
    faturamento_total += valor_final;

    // Atualização dos limites de menor e maior frete registrado
    if (total_encomendas == 1) { // Primeira encomenda do dia inicializa ambos os valores
        maior_frete = valor_final;
        menor_frete = valor_final;
    } else {
        if (valor_final > maior_frete) maior_frete = valor_final;
        if (valor_final < menor_frete) menor_frete = valor_final;
    }

    // Exibição do comprovante na tela
    printf("\n===========================================================\n");
    printf("               LOGTECH - COMPROVANTE DE FRETE              \n");
    printf("===========================================================\n");
    printf("Encomenda N°             : %03d\n", total_encomendas);
    
    // Imprime o nome da categoria usando seleções com if/else
    printf("Categoria Selecionada    : ");
    if (categoria == 1) {
        printf("Padrao\n");
    } else if (categoria == 2) {
        printf("Expressa\n");
    } else if (categoria == 3) {
        printf("Prioritaria\n");
    }

    printf("Peso Registrado          : %.2f kg\n", peso);
    printf("Distancia Percorrida     : %.2f km\n", distancia);

    // Identificação do cupom/isenção aplicado no comprovante
    if (frete_gratis == 1) {
        printf("Cupom Aplicado           : Isento (Frete Gratis)\n");
    } else if (cupom == 1020) {
        printf("Cupom Aplicado           : 1020 (LOG10 - 10%% OFF)\n");
    } else if (cupom == 2030) {
        printf("Cupom Aplicado           : 2030 (DESCONTO15)\n");
    } else {
        printf("Cupom Aplicado           : Nenhum\n");
    }

    printf("-----------------------------------------------------------\n");
    printf("Valor Bruto do Frete     : R$ %.2f\n", frete_bruto);
    printf("Valor Final Calculado    : R$ %.2f\n", valor_final);
    printf("Status                   : Processado com Sucesso\n");
    printf("===========================================================\n\n");
}

// ==========================================
// RELATÓRIO: RESUMO ESTATÍSTICO DO DIA
// Exibe os totais e médias calculadas
// ==========================================
void registro()
{
    // Validação caso nenhuma encomenda tenha sido processada ainda
    if (total_encomendas == 0) {
        printf("\nNenhuma encomenda processada ate o momento!\n\n");
        return;
    }

    // Cálculo das médias diárias
    float media_peso = soma_peso / total_encomendas;
    float media_distancia = soma_distancia / total_encomendas;

    printf("\n=========================================\n");
    printf("     RESUMO FINANCEIRO E OPERACIONAL     \n");
    printf("=========================================\n");
    printf("Total de encomendas processadas : %d\n", total_encomendas);
    printf("Faturamento total em fretes     : R$ %.2f\n", faturamento_total);
    printf("Media de peso das encomendas    : %.2f kg\n", media_peso);
    printf("Media de distancia percorrida   : %.2f km\n", media_distancia);
    printf("-----------------------------------------\n");
    printf("Maior frete individual cobrado : R$ %.2f\n", maior_frete);
    printf("Menor frete individual cobrado : R$ %.2f\n", menor_frete);
    printf("Total de isencoes (Frete Gratis): %d encomendas\n", qtd_isencoes);
    printf("=========================================\n\n");
}

// ==========================================
// RELATÓRIO: INDICADORES E ALERTAS
// Exibe verificação de meta de faturamento e categorias
// ==========================================
void indicadores()
{
    // Validação caso não haja cadastro
    if (total_encomendas == 0) {
        printf("\nNenhuma encomenda cadastrada para calcular indicadores!\n\n");
        return;
    }

    // Percentual de entregas gratuitas sobre o total
    float taxa_isencao = ((float)qtd_isencoes / total_encomendas) * 100.0;

    printf("\n======================================\n");
    printf("   INDICADORES E ALERTAS OPERACIONAIS \n");
    printf("======================================\n");
    printf("Meta de Faturamento (R$ 1000.00) : R$ %.2f\n", faturamento_total);

    // Comparação do faturamento atual com a meta de R$ 1000.00
    if (faturamento_total >= META_FATURAMENTO) {
        printf("Status da Meta                   : META ATINGIDA!\n");
    } else {
        printf("Status da Meta                   : Faltam R$ %.2f\n", META_FATURAMENTO - faturamento_total);
    }

    // Identificação da categoria com maior volume de pedidos
    printf("Categoria mais Utilizada no Dia  : ");
    if (qtd_padrao >= qtd_expressa && qtd_padrao >= qtd_prioritaria) {
        printf("Padrao\n");
    } else if (qtd_expressa >= qtd_padrao && qtd_expressa >= qtd_prioritaria) {
        printf("Expressa\n");
    } else {
        printf("Prioritaria\n");
    }

    printf("Taxa de Entregas com Frete Gratis: %.1f %%\n", taxa_isencao);
    printf("======================================\n\n");
}