// Bibliotecas
#include <stdio.h> 
#include <string.h>

// Defines
#define MAX_ENCOMENDAS 100
#define META_FATURAMENTO 1000.00

// Struct para armazenar cada encomenda
typedef struct {
    int id;                 // ID unico da encomenda
    int categoria;          // Categoria (1- Padrao, 2- Expressa, 3- Prioritaria)
    int cupom;              // Codigo do cupom usado
    float peso;             // Peso em kg
    float distancia;        // Distancia em km
    float valor_bruto;      // Valor do frete sem descontos
    float valor_final;     // Valor cobrado do cliente 
    char status[15];        // Status: "ATIVA" ou "CANCELADA"
} typeOfencomenda;

typeOfencomenda encomendas[MAX_ENCOMENDAS]; // Vetor de encomendas

// =========================================================================================================================================================
// VARIÁVEIS GLOBAIS
// Acumulam dados ao longo da execução do programa
// =========================================================================================================================================================

int total_encomendas = 0;   // Contador de encomendas registradas
int qtd_padrao = 0;         // Quantidade de encomendas da categoria Padrão
int qtd_expressa = 0;       // Quantidade de encomendas da categoria Expressa
int qtd_prioritaria = 0;    // Quantidade de encomendas da categoria Prioritária
int qtd_isencoes = 0;       // Quantidade de encomendas com frete grátis

float faturamento_total = 0.0; // Soma do valor total arrecadado
float soma_peso = 0.0;         // Soma dos pesos para calcular a média
float soma_distancia = 0.0;    // Soma das distâncias para calcular a média
float maior_frete = 0.0;       // Guarda o maior valor de frete calculado
float menor_frete = 0.0;       // Guarda o menor valor de frete calculado

int pacote_atual = 0;
// =========================================================================================================================================================
// PROTÓTIPOS DAS FUNÇÕES
// Declaração antecipada das funções do código
// =========================================================================================================================================================

void programa();
void cadastro();
void registro();
void indicadores();
void listar_encomendas();
void buscar_por_id();
void cancelar_encomenda();
void exportar_relatorio();
void limpar_historico();
void processar_encomenda(int categoria, float peso, float distancia, int cupom);

int solicitar_cupom();
float calcular_taxa_peso(float peso);
int verificar_frete_gratis(int categoria, float peso, float distancia);

void salvar_arquivo();
void carregar_arquivo();
void recalcular_globais();

// Função principal de entrada da execução
int main(int argc, char **argv)
{
    carregar_arquivo(); // Carrega o historico do arquivo encomendas.txt
    programa();         // Inicia o menu principal do programa
    return 0;           // Retorna 0 indicando execução com sucesso
}

// Recalcula as variáveis globais com base nos itens ATIVOS
void recalcular_globais()
{
    faturamento_total = 0.0;
    soma_peso = 0.0;
    soma_distancia = 0.0;
    qtd_padrao = 0;
    qtd_expressa = 0;
    qtd_prioritaria = 0;
    qtd_isencoes = 0;
    maior_frete = 0.0;
    menor_frete = 0.0;

    int primeiro_ativo = 1;

    for (int i = 0; i < total_encomendas; i++) {
        if (strcmp(encomendas[i].status, "ATIVA") == 0) {
            faturamento_total += encomendas[i].valor_final;
            soma_peso += encomendas[i].peso;
            soma_distancia += encomendas[i].distancia;

            if (encomendas[i].categoria == 1) qtd_padrao++;
            else if (encomendas[i].categoria == 2) qtd_expressa++;
            else if (encomendas[i].categoria == 3) qtd_prioritaria++;

            if (encomendas[i].valor_final == 0.0) {
                qtd_isencoes++;
            }

            if (primeiro_ativo) {
                maior_frete = encomendas[i].valor_final;
                menor_frete = encomendas[i].valor_final;
                primeiro_ativo = 0;
            } else {
                if (encomendas[i].valor_final > maior_frete) maior_frete = encomendas[i].valor_final;
                if (encomendas[i].valor_final < menor_frete) menor_frete = encomendas[i].valor_final;
            }
        }
    }
}

// Salva todo o vetor no arquivo encomendas.txt
void salvar_arquivo()
{
    FILE *arq = fopen("encomendas.txt", "w");
    if (arq == NULL) {
        printf("\nErro ao abrir arquivo para salvar!\n");
        return;
    }

    for (int i = 0; i < total_encomendas; i++) {
        fprintf(arq, "ID:%03d | TIPO:%d | PESO:%.2f | DIST:%.2f | CUPOM:%d | BRUTO:%.2f | FINAL:%.2f | STATUS:%s\n",
                encomendas[i].id,
                encomendas[i].categoria,
                encomendas[i].peso,
                encomendas[i].distancia,
                encomendas[i].cupom,
                encomendas[i].valor_bruto,
                encomendas[i].valor_final,
                encomendas[i].status);
    }

    fclose(arq);
}

// Lê o arquivo encomendas.txt ao abrir o sistema
void carregar_arquivo()
{
    FILE *arq = fopen("encomendas.txt", "r");
    if (arq == NULL) {
        return; // Arquivo ainda não existe
    }

    total_encomendas = 0;

    while (!feof(arq) && total_encomendas < MAX_ENCOMENDAS) {
        typeOfencomenda e;
        int lidos = fscanf(arq, "ID:%d | TIPO:%d | PESO:%f | DIST:%f | CUPOM:%d | BRUTO:%f | FINAL:%f | STATUS:%s\n",
                           &e.id, &e.categoria, &e.peso, &e.distancia, &e.cupom, &e.valor_bruto, &e.valor_final, e.status);
        
        if (lidos == 8) {
            encomendas[total_encomendas] = e;
            total_encomendas++;
        }
    }

    fclose(arq);
    recalcular_globais();
}

// =========================================================================================================================================================
// MENU PRINCIPAL DO SISTEMA
// =========================================================================================================================================================
void programa()
{ 
    int opcao;

    for (;;) 
    {
        printf("=====================================================\n");
        printf("LOGTECH SGEF - VERSAO 2.0\n");
        printf("=====================================================\n");
        printf("1-Cadastrar e Processar Encomenda\n");
        printf("2-Listar Encomendas Registradas\n");
        printf("3-Buscar Encomendas por ID\n");
        printf("4-Cancelar Encomenda\n");
        printf("5-Exibir Resumo Estatistico e Indicadores\n");
        printf("6-Exportar Relatorio Diario (.txt)\n");
        printf("7-Limpar/Reiniciar Historico do Dia\n");
        printf("8-Sair\n");
        printf("=====================================================\n");
        printf("Escolha uma opcao: ");
        
        if (scanf("%d", &opcao) != 1) {
            printf("\nEntrada invalida!\n\n");
            while (getchar() != '\n'); 
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
            listar_encomendas();
            break;

        case 3:
            buscar_por_id();
            break;

        case 4:
            cancelar_encomenda();
            break;

        case 5:
            registro();    
            indicadores(); 
            break;

        case 6:
            exportar_relatorio();
            break;

        case 7:
            limpar_historico();
            break;

        case 8:
            printf("\nSaindo do Sistema...\n");
            return; 

        default:
            printf("\nOpcao Invalida!\n\n");
            break;
        }
    }
}

// ========================================================================================================================================================
// CADASTRO E VALIDAÇÃO DE DADOS
// ========================================================================================================================================================
void cadastro()
{
    char tem_cupom = 'N';
    int codigo_cupom = 0;

    printf("\n======================================\n CADASTRAR PACOTE \n======================================\n");

    do {
        printf("Qual a Categoria do Pacote? (1- Padrao, 2- Expresso, 3- Prioritario): ");
        if (scanf("%d", &encomendas[pacote_atual].categoria) != 1) {
            while (getchar() != '\n');
            encomendas[pacote_atual].categoria = 0;
        }
        if ( encomendas[pacote_atual].categoria < 1 ||  encomendas[pacote_atual].categoria > 3) {
            printf("Categoria invalida! Escolha 1, 2 ou 3.\n");
        }
    } while ( encomendas[pacote_atual].categoria < 1 ||  encomendas[pacote_atual].categoria > 3);

    do {
        printf("Qual o peso do pacote (EM KG): ");
        if (scanf("%f", &encomendas[pacote_atual].peso) != 1) {
            while (getchar() != '\n');
             encomendas[pacote_atual].peso = 0.0;
        }
        if (encomendas[pacote_atual].peso <= 0) {
            printf("O peso deve ser estritamente maior que 0 kg!\n");
        }
    } while (encomendas[pacote_atual].peso <= 0);

    do {
        printf("Me diga a distancia do pedido (Em KM): ");
        if (scanf("%f", &encomendas[pacote_atual].distancia) != 1) {
            while (getchar() != '\n');
            encomendas[pacote_atual].distancia = 0.0;
        }
        if (encomendas[pacote_atual].distancia <= 0) {
            printf("A distancia deve ser estritamente maior que 0 km!\n");
        }
    } while (encomendas[pacote_atual].distancia <= 0);

    do {
        printf("Teve cupom? (S/N): ");
        scanf(" %c", &tem_cupom);
        if (tem_cupom != 'S' && tem_cupom != 's' && tem_cupom != 'N' && tem_cupom != 'n') {
            printf("Opção invalida! Digite S para Sim ou N para Nao.\n");
        }
    } while (tem_cupom != 'S' && tem_cupom != 's' && tem_cupom != 'N' && tem_cupom != 'n');

    if (tem_cupom == 'S' || tem_cupom == 's') {
        encomendas[pacote_atual].cupom = solicitar_cupom();
    }

    processar_encomenda(encomendas[pacote_atual].categoria, encomendas[pacote_atual].peso, encomendas[pacote_atual].distancia, encomendas[pacote_atual].cupom);
}

int solicitar_cupom()
{
    int cupom = 0;
    while (1) {
        printf("Qual o codigo do cupom (1020 ou 2030, ou 0 para cancelar): ");
        if (scanf("%d", &cupom) != 1) {
            while (getchar() != '\n');
            cupom = -1;
        }

        if (cupom == 1020 || cupom == 2030 || cupom == 0) {
            return cupom; 
        } else {
            printf("Codigo de cupom invalido! Tente novamente.\n");
        }
    }
}

float calcular_taxa_peso(float peso)
{
    if (peso <= 5.0) {
        return peso * 2.00;
    } else {
        return peso * 3.50;
    }
}

int verificar_frete_gratis(int categoria, float peso, float distancia)
{
    if (categoria == 1 && peso <= 2.0 && distancia <= 10.0) {
        return 1;
    } else {
        return 0;
    }
}

void processar_encomenda(int categoria, float peso, float distancia, int cupom)
{
    float multiplicador_dist = 0.0;
    float taxa_adicional = 0.0;

    switch (categoria) {
    case 1:
        multiplicador_dist = 1.20;
        taxa_adicional = 0.0;
        break;
    case 2:
        multiplicador_dist = 1.80;
        taxa_adicional = 15.00;
        break;
    case 3:
        multiplicador_dist = 2.50;
        taxa_adicional = 30.00;
        break;
    }

    float valor_peso = calcular_taxa_peso(peso);
    float frete_bruto = (distancia * multiplicador_dist) + valor_peso + taxa_adicional;
    float valor_final = frete_bruto;

    int frete_gratis = verificar_frete_gratis(categoria, peso, distancia);

    if (frete_gratis == 1) {
        valor_final = 0.0;
    } else {
        if (distancia > 100.0) {
            valor_final -= (frete_bruto * 0.10);
        }
        if (cupom == 1020) {
            valor_final -= (valor_final * 0.10);
        } else if (cupom == 2030) {
            valor_final -= 15.00;
            if (valor_final < 0.0) valor_final = 0.0;
        }
    }

    // Guarda a encomenda no vetor struct
    int idx = total_encomendas;
    encomendas[idx].id = total_encomendas + 1;
    encomendas[idx].categoria = categoria;
    encomendas[idx].peso = peso;
    encomendas[idx].distancia = distancia;
    encomendas[idx].cupom = cupom;
    encomendas[idx].valor_bruto = frete_bruto;
    encomendas[idx].valor_final = valor_final;
    strcpy(encomendas[idx].status, "ATIVA");

    total_encomendas++;

    recalcular_globais();
    salvar_arquivo();

    // Comprovante impresso na tela
    printf("\n===========================================================\n");
    printf("               LOGTECH - COMPROVANTE DE FRETE              \n");
    printf("===========================================================\n");
    printf("Encomenda N°             : %03d\n", encomendas[idx].id);
    
    printf("Categoria Selecionada    : ");
    if (categoria == 1) printf("Padrao\n");
    else if (categoria == 2) printf("Expressa\n");
    else if (categoria == 3) printf("Prioritaria\n");

    printf("Peso Registrado          : %.2f kg\n", peso);
    printf("Distancia Percorrida     : %.2f km\n", distancia);

    if (frete_gratis == 1) {
        printf("Cupom Aplicado           : Isento (Frete Gratis)\n");
    } else if (cupom == 1020) {
        printf("Cupom Aplicado           : 1020 (LOG10 - 10%% OFF)\n");
    } else if (cupom == 2030) {
        printf("Cupom Aplicado           : 2030 (DESCONTO15)\n");
    } else {
        printf("Cupom Aplicado           : Nenhum\n");
    }

    printf("Valor Bruto do Frete     : R$ %.2f\n", frete_bruto);
    printf("Valor Final Calculado    : R$ %.2f\n", valor_final);
    printf("Status                   : ATIVA\n");
    printf("===========================================================\n\n");
}

// ========================================================================================================================================================
// LISTAGEM, BUSCA E CANCELAMENTO DE ENCOMENDAS
// ========================================================================================================================================================

void listar_encomendas()
{
    if (total_encomendas == 0) {
        printf("\nNenhuma encomenda cadastrada ate o momento!\n\n");
        return;
    }

    printf("\n===================================================================\n");
    printf("                 LISTA DE ENCOMENDAS REGISTRADAS                   \n");
    printf("===================================================================\n");
    for (int i = 0; i < total_encomendas; i++) {
        printf("ID: %03d | Cat: %d | Peso: %.2fkg | Dist: %.2fkm | R$ %.2f | Status: %s\n",
               encomendas[i].id,
               encomendas[i].categoria,
               encomendas[i].peso,
               encomendas[i].distancia,
               encomendas[i].valor_final,
               encomendas[i].status);
    }
    printf("===================================================================\n\n");
}

void buscar_por_id()
{
    if (total_encomendas == 0) {
        printf("\nNenhuma encomenda cadastrada!\n\n");
        return;
    }

    int id;
    printf("\nDigite o ID da encomenda: ");
    if (scanf("%d", &id) != 1) {
        printf("\nID invalido!\n\n");
        while (getchar() != '\n');
        return;
    }

    int achou = 0;
    for (int i = 0; i < total_encomendas; i++) {
        if (encomendas[i].id == id) {
            achou = 1;
            printf("\n========== ENCOMENDA ==========\n");
            printf("ID: %03d\n", encomendas[i].id);

            printf("Categoria: ");
            if (encomendas[i].categoria == 1) printf("Padrao\n");
            else if (encomendas[i].categoria == 2) printf("Expressa\n");
            else if (encomendas[i].categoria == 3) printf("Prioritaria\n");

            printf("Peso: %.2f kg\n", encomendas[i].peso);
            printf("Distancia: %.2f km\n", encomendas[i].distancia);
            printf("Cupom: %d\n", encomendas[i].cupom);
            printf("Valor bruto: R$ %.2f\n", encomendas[i].valor_bruto);
            printf("Valor final: R$ %.2f\n", encomendas[i].valor_final);
            printf("Status: %s\n", encomendas[i].status);
            printf("===============================\n\n");
            break;
        }
    }

    if (!achou) {
        printf("\nEncomenda nao encontrada!\n\n");
    }
}

void cancelar_encomenda()
{
    if (total_encomendas == 0) {
        printf("\nNenhuma encomenda cadastrada para cancelar!\n\n");
        return;
    }

    int id;
    printf("\nDigite o ID da encomenda que deseja cancelar: ");
    if (scanf("%d", &id) != 1) {
        printf("\nID invalido!\n\n");
        while (getchar() != '\n');
        return;
    }

    int achou = 0;
    for (int i = 0; i < total_encomendas; i++) {
        if (encomendas[i].id == id) {
            achou = 1;
            if (strcmp(encomendas[i].status, "CANCELADA") == 0) {
                printf("\nEsta encomenda ja esta cancelada!\n\n");
            } else {
                strcpy(encomendas[i].status, "CANCELADA");
                recalcular_globais();
                salvar_arquivo();

                printf("\nEncomenda %03d cancelada com sucesso!\n", encomendas[i].id);
                printf("O registro foi mantido no historico.\n\n");
            }
            break;
        }
    }

    if (!achou) {
        printf("\nEncomenda nao encontrada!\n\n");
    }
}

// ========================================================================================================================================================
// RELATÓRIOS E INDICADORES
// ========================================================================================================================================================

void registro()
{
    if (total_encomendas == 0) {
        printf("\nNenhuma encomenda processada ate o momento!\n\n");
        return;
    }

    int ativas = 0;
    for (int i = 0; i < total_encomendas; i++) {
        if (strcmp(encomendas[i].status, "ATIVA") == 0) {
            ativas++;
        }
    }

    float media_peso = (ativas > 0) ? (soma_peso / ativas) : 0.0;
    float media_distancia = (ativas > 0) ? (soma_distancia / ativas) : 0.0;

    printf("\n=========================================\n");
    printf("     LOGTECH SGEF - VERSAO 2.0     \n");
    printf("=========================================\n");
    printf("Total de Encomendas Processadas : %d\n", total_encomendas);
    printf("Total de Encomendas Ativas      : %d\n", ativas);
    printf("Total de Encomendas Canceladas  : %d\n", total_encomendas - ativas);
    printf("-----------------------------------------\n");
    printf("Faturamento Total em Fretes     : R$ %.2f\n", faturamento_total);
    printf("Media de peso (Encomendas Ativas): %.2f kg\n", media_peso);
    printf("Media de distancia (Ativas)     : %.2f km\n", media_distancia);
    printf("-----------------------------------------\n");
    printf("Maior frete individual cobrado  : R$ %.2f\n", maior_frete);
    printf("Menor frete individual cobrado  : R$ %.2f\n", menor_frete);
    printf("Total de isencoes (Frete Gratis): %d encomendas\n", qtd_isencoes);
    printf("=========================================\n");
}

void indicadores()
{
    if (total_encomendas == 0) {
        return;
    }

    int ativas = 0;
    for (int i = 0; i < total_encomendas; i++) {
        if (strcmp(encomendas[i].status, "ATIVA") == 0) {
            ativas++;
        }
    }

    float taxa_isencao = (ativas > 0) ? (((float)qtd_isencoes / ativas) * 100.0) : 0.0;

    printf("\n======================================\n");
    printf("   INDICADORES E ALERTAS OPERACIONAIS \n");
    printf("======================================\n");
    printf("Meta de Faturamento (R$ 1000.00) : R$ %.2f\n", faturamento_total);

    if (faturamento_total >= META_FATURAMENTO) {
        printf("Status da Meta                   : META ATINGIDA!\n");
    } else {
        printf("Status da Meta                   : Faltam R$ %.2f\n", META_FATURAMENTO - faturamento_total);
    }

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

// Opção 6: Salva relatorio_diario.txt
void exportar_relatorio()
{
    if (total_encomendas == 0) {
        printf("\nNenhuma encomenda para gerar relatorio!\n\n");
        return;
    }

    char data_str[15];
    printf("\nDigite a data de hoje (ex: 15/10/2026): ");
    scanf("%s", data_str);

    FILE *rel = fopen("relatorio_diario.txt", "w");
    if (rel == NULL) {
        printf("\nErro ao criar relatorio_diario.txt!\n\n");
        return;
    }

    int ativas = 0;
    for (int i = 0; i < total_encomendas; i++) {
        if (strcmp(encomendas[i].status, "ATIVA") == 0) {
            ativas++;
        }
    }

    float media_p = (ativas > 0) ? (soma_peso / ativas) : 0.0;
    float media_d = (ativas > 0) ? (soma_distancia / ativas) : 0.0;
    float taxa_is = (ativas > 0) ? (((float)qtd_isencoes / ativas) * 100.0) : 0.0;

    fprintf(rel, "=========================================\n");
    fprintf(rel, "     LOGTECH LOGISTICS - RELATORIO DIARIO\n");
    fprintf(rel, "=========================================\n");
    fprintf(rel, "Data da Exportacao              : %s\n", data_str);
    fprintf(rel, "Total de Encomendas Processadas : %d\n", total_encomendas);
    fprintf(rel, "Total de Encomendas Ativas      : %d\n", ativas);
    fprintf(rel, "Total de Encomendas Canceladas  : %d\n", total_encomendas - ativas);
    fprintf(rel, "-----------------------------------------\n");
    fprintf(rel, "Faturamento Total em Fretes     : R$ %.2f\n", faturamento_total);
    fprintf(rel, "Media de Peso (Encomendas Ativas): %.2f kg\n", media_p);
    fprintf(rel, "Media de Distancia (Ativas)     : %.2f km\n", media_d);
    fprintf(rel, "-----------------------------------------\n");
    fprintf(rel, "Maior Frete Individual Cobrado  : R$ %.2f\n", maior_frete);
    fprintf(rel, "Menor Frete Individual Cobrado  : R$ %.2f\n", menor_frete);
    fprintf(rel, "Total de Isencoes (Frete Gratis): %d encomenda(s)\n", qtd_isencoes);
    fprintf(rel, "-----------------------------------------\n");
    fprintf(rel, "   INDICADORES E ALERTAS OPERACIONAIS    \n");
    fprintf(rel, "-----------------------------------------\n");
    
    if (faturamento_total >= META_FATURAMENTO) {
        fprintf(rel, "Meta de Faturamento (R$ 1000.00): R$ %.2f (META ATINGIDA!)\n", faturamento_total);
    } else {
        fprintf(rel, "Meta de Faturamento (R$ 1000.00): R$ %.2f (Faltam R$ %.2f)\n", faturamento_total, META_FATURAMENTO - faturamento_total);
    }

    fprintf(rel, "Categoria mais utilizada no dia : ");
    if (qtd_padrao >= qtd_expressa && qtd_padrao >= qtd_prioritaria) {
        fprintf(rel, "Padrao\n");
    } else if (qtd_expressa >= qtd_padrao && qtd_expressa >= qtd_prioritaria) {
        fprintf(rel, "Expressa\n");
    } else {
        fprintf(rel, "Prioritaria\n");
    }

    fprintf(rel, "Taxa de Entregas com Frete Gratis: %.1f %%\n", taxa_is);
    fprintf(rel, "=========================================\n");

    fclose(rel);
    printf("\nRelatorio salvo com sucesso em 'relatorio_diario.txt'!\n\n");
}

// Opção 7: Limpa a memória e apaga o arquivo físico
void limpar_historico()
{
    char confirmacao;
    printf("\nDeseja realmente apagar todo o historico acumulado? (S/N): ");
    scanf(" %c", &confirmacao);

    if (confirmacao == 'S' || confirmacao == 's') {
        total_encomendas = 0;
        recalcular_globais();

        FILE *arq = fopen("encomendas.txt", "w");
        if (arq != NULL) {
            fclose(arq);
        }

        printf("\nHistorico do dia limpo com sucesso!\n\n");
    } else {
        printf("\nOperacao cancelada.\n\n");
    }
}
