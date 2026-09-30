#include <stdio.h>
#include <stdbool.h>

#define MAX_ENCOMENDAS 100
#define META_FATURAMENTO 1000.00

// Variáveis Globais
int total_encomendas = 0;
float faturamento_total = 0.0;
float soma_peso = 0.0;
float soma_distancia = 0.0;

float maior_frete = 0.0;
float menor_frete = 0.0;

int qtd_padrao = 0;
int qtd_expressa = 0;
int qtd_prioritaria = 0;
int qtd_isencoes = 0;

// Protótipos das Funções
void programa();
void cadastro();
int solicitar_cupom();
float calcular_taxa_peso(float peso);
bool verificar_frete_gratis(int categoria, float peso, float distancia);
void processar_encomenda(int categoria, float peso, float distancia, int cupom);
void registro();
void indicadores();

int main(int argc, char **argv)
{
    programa();
    return 0;
}

// Menu do Sistema
void programa()
{
    int opcao;

    for (;;)
    {
        printf("======================================\n");
        printf("LOGTECH LOGISTICS MENU\n");
        printf("======================================\n");
        printf("1-Cadastrar e Processar encomenda\n");
        printf("2-Exibir Resumo Estatistico do dia\n");
        printf("3-Exibir Indicadores\n");
        printf("4-Sair\n");
        printf("Escolha uma opcao: ");
        
        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n'); // Limpa buffer
            printf("\nEntrada invalida!\n\n");
            continue;
        }

        switch (opcao)
        {
        case 1:
            if (total_encomendas >= MAX_ENCOMENDAS) {
                printf("\nLimite maximo de 100 encomendas atingido para hoje!\n\n");
            } else {
                cadastro();
            }
            break;

        case 2:
            registro();
            break;

        case 3:
            indicadores();
            break;

        case 4:
            printf("\nSaindo do Sistema...\n");
            return;

        default:
            printf("\nOpcao Invalida!\n\n");
            break;
        }
    }
}

// Leitura e Validação do Cadastro
void cadastro()
{
    int categoria = 0;
    float peso = 0.0, distancia = 0.0;
    char tem_cupom = 'N';
    int codigo_cupom = 0;

    printf("\n======================================\n CADASTRAR PACOTE \n======================================\n");

    // Validação da Categoria
    do {
        printf("Qual a Categoria do Pacote? (1- Padrao, 2- Expresso, 3- Prioritario): ");
        scanf("%d", &categoria);
        if (categoria < 1 || categoria > 3) {
            printf("Categoria invalida! Escolha 1, 2 ou 3.\n");
        }
    } while (categoria < 1 || categoria > 3);

    // Validação do Peso (> 0 kg)
    do {
        printf("Qual o peso do pacote (EM KG): ");
        scanf("%f", &peso);
        if (peso <= 0) {
            printf("O peso deve ser estritamente maior que 0 kg!\n");
        }
    } while (peso <= 0);

    // Validação da Distância (> 0 km)
    do {
        printf("Me diga a distancia do pedido (Em KM): ");
        scanf("%f", &distancia);
        if (distancia <= 0) {
            printf("A distancia deve ser estritamente maior que 0 km!\n");
        }
    } while (distancia <= 0);

    // Pergunta do Cupom
    printf("Teve cupom? (S/N): ");
    scanf(" %c", &tem_cupom);

    if (tem_cupom == 'S' || tem_cupom == 's') {
        codigo_cupom = solicitar_cupom();
    }

    processar_encomenda(categoria, peso, distancia, codigo_cupom);
}

// Solicitação e Validação do Cupom
int solicitar_cupom()
{
    int cupom = 0;
    while (1) {
        printf("Qual o codigo do cupom (1020 ou 2030, ou 0 para cancelar): ");
        scanf("%d", &cupom);

        if (cupom == 1020 || cupom == 2030 || cupom == 0) {
            return cupom;
        } else {
            printf("Codigo de cupom invalido! Tente novamente.\n");
        }
    }
}

// Taxa de Peso
float calcular_taxa_peso(float peso)
{
    if (peso <= 5.0) {
        return peso * 2.00;
    } else {
        return peso * 3.50;
    }
}

// Regra de Isenção (Frete Grátis)
bool verificar_frete_gratis(int categoria, float peso, float distancia)
{
    return (categoria == 1 && peso <= 2.0 && distancia <= 10.0);
}

// Processamento da Encomenda e Cálculo do Frete
void processar_encomenda(int categoria, float peso, float distancia, int cupom)
{
    float multiplicador_dist = 0.0;
    float taxa_adicional = 0.0;
    char nome_categoria[20];

    switch (categoria) {
    case 1:
        multiplicador_dist = 1.20;
        taxa_adicional = 0.0;
        sprintf(nome_categoria, "Padrao");
        qtd_padrao++;
        break;
    case 2:
        multiplicador_dist = 1.80;
        taxa_adicional = 15.00;
        sprintf(nome_categoria, "Expressa");
        qtd_expressa++;
        break;
    case 3:
        multiplicador_dist = 2.50;
        taxa_adicional = 30.00;
        sprintf(nome_categoria, "Prioritaria");
        qtd_prioritaria++;
        break;
    }

    float valor_peso = calcular_taxa_peso(peso);
    float frete_bruto = (distancia * multiplicador_dist) + valor_peso + taxa_adicional;
    float valor_final = frete_bruto;

    bool frete_gratis = verificar_frete_gratis(categoria, peso, distancia);

    if (frete_gratis) {
        valor_final = 0.0;
        qtd_isencoes++;
    } else {
        // Desconto por distância (> 100km dá 10% de desconto no frete bruto)
        if (distancia > 100.0) {
            valor_final -= (frete_bruto * 0.10);
        }

        // Desconto por cupom
        if (cupom == 1020) { // LOG10 (10% OFF)
            valor_final -= (valor_final * 0.10);
        } else if (cupom == 2030) { // DESCONTO15 (R$ 15,00 OFF)
            valor_final -= 15.00;
            if (valor_final < 0.0) valor_final = 0.0;
        }
    }

    total_encomendas++;
    soma_peso += peso;
    soma_distancia += distancia;
    faturamento_total += valor_final;

    // Controle de maior e menor frete cobrado
    if (total_encomendas == 1) {
        maior_frete = valor_final;
        menor_frete = valor_final;
    } else {
        if (valor_final > maior_frete) maior_frete = valor_final;
        if (valor_final < menor_frete) menor_frete = valor_final;
    }

    // Impressão do Comprovante
    printf("\n===========================================================\n");
    printf("               LOGTECH - COMPROVANTE DE FRETE              \n");
    printf("===========================================================\n");
    printf("Encomenda N°             : %03d\n", total_encomendas);
    printf("Categoria Selecionada    : %s\n", nome_categoria);
    printf("Peso Registrado          : %.2f kg\n", peso);
    printf("Distancia Percorrida     : %.2f km\n", distancia);

    if (frete_gratis) {
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

// Opção 2: Resumo Estatístico
void registro()
{
    if (total_encomendas == 0) {
        printf("\nNenhuma encomenda processada ate o momento!\n\n");
        return;
    }

    float media_peso = soma_peso / total_encomendas;
    float media_distancia = soma_distancia / total_encomendas;

    printf("\n=========================================\n");
    printf("     RESUMO FINANCEIRO E OPERACIONAL     \n");
    printf("=========================================\n");
    printf("Total de encomendas processadas : %d\n", total_encomendas);
    printf("Faturamento total em fretes     : R$ %.2f\n", faturamento_total);
    printf("Media de peso das encomendas   : %.2f kg\n", media_peso);
    printf("Media de distancia percorrida   : %.2f km\n", media_distancia);
    printf("-----------------------------------------\n");
    printf("Maior frete individual cobrado : R$ %.2f\n", maior_frete);
    printf("Menor frete individual cobrado : R$ %.2f\n", menor_frete);
    printf("Total de isencoes (Frete Gratis): %d encomendas\n", qtd_isencoes);
    printf("=========================================\n\n");
}

// Opção 3: Indicadores Operacionais
void indicadores()
{
    if (total_encomendas == 0) {
        printf("\nNenhuma encomenda cadastrada para calcular indicadores!\n\n");
        return;
    }

    float taxa_isencao = ((float)qtd_isencoes / total_encomendas) * 100.0;

    printf("\n======================================\n");
    printf("   INDICADORES E ALERTAS OPERACIONAIS \n");
    printf("======================================\n");
    printf("Meta de Faturamento (R$ 1000.00) : R$ %.2f\n", faturamento_total);

    if (faturamento_total >= META_FATURAMENTO) {
        printf("Status da Meta                   : META ATINGIDA!\n");
    } else {
        printf("Status da Meta                   : Faltam R$ %.2f\n", META_FATURAMENTO - faturamento_total);
    }

    // Categoria mais utilizada
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