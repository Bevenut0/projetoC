#include <stdio.h>
#include <stdbool.h>
#define maximo_encomendas 100

int encomenda = 0;
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

float gerenciandoCategorias(int categoria, float distancia, float taxaDePeso, float peso, bool freteGratis);
float gerenciandoPeso(float peso);
bool gerenciandoFrete(int categoria, float distancia, float peso);
void cadastro();
void programa();
int registro(float somaPeso, int encomenda, float somaDistancia, float freteTotal);

int main(int argc, char **argv)
{
    programa();
    return 0;
}

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
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            printf("\n======================================\n CADASTRAR PACOTE \n======================================\n");
            cadastro();
            break;

        case 2:
            printf("\nExibindo Resumo...\n");
            // Passando os parâmetros acumulados para a função de registro
            registro(soma_peso, encomenda, soma_distancia, faturamento_total);
            break;

        case 3:
            printf("\nExibindo Indicadores... (Em construcao)\n");
            break;

        case 4:
            printf("\nSaindo do Sistema...\n");
            return;

        default:
            printf("\nOpcao Invalida!\n");
            break;
        }
    }
}

void cadastro()
{
    int categoria;
    float peso, distancia;
    char cupom;
    printf("Qual a Categoria do Pacote? (1, 2 ou 3):\n");
    scanf("%d", &categoria);

    printf("Qual o peso do pacote (EM KG): ");
    scanf("%f", &peso);

    float taxaDePeso = gerenciandoPeso(peso);

    printf("Me diga a distancia do pedido (Em KM): ");
    scanf("%f", &distancia);

    printf("Teve cupom? (S/N): ");
    scanf(" %c", &cupom); // Espaço antes de %c ignora o 'Enter' pendente
    
    bool freteGratis = gerenciandoFrete(categoria, distancia, peso);

    float valorCalculado = gerenciandoCategorias(categoria, distancia, taxaDePeso, peso, freteGratis);
}

bool gerenciandoFrete(int categoria, float distancia, float peso)
{
    if (categoria == 1 && peso <= 2 && distancia < 10)
    {
        return true;
    }
    return false; 
}

float gerenciandoPeso(float peso)
{
    if (peso > 5.0)
    {
        return peso * 3.50;
    }
    else
    {
        return peso * 2.0;
    }
}

float gerenciandoCategorias(int categoria, float distancia, float taxaDePeso, float peso, bool freteGratis)
{
    float multiplicadorDistancia = 0, freteBruto = 0, valorFinal = 0, cupom = 0;

    switch (categoria)
    {
    case 1:
        multiplicadorDistancia = 1.20 * distancia;
        freteBruto = (multiplicadorDistancia * distancia) + (peso * taxaDePeso);
        valorFinal = freteBruto - cupom;
        encomenda++;

        // Atualizando os acumuladores globais
        soma_peso += peso;
        soma_distancia += distancia;
        faturamento_total += valorFinal;

        printf("\n===========================================================\n");
        printf("               LOGTECH - COMPROVANTE DE FRETE              \n");
        printf("===========================================================\n");
        printf("Encomenda N              : %03d\n", encomenda);
        printf("Categoria Selecionada    : Padrao\n");
        printf("Peso Registrado          : %.2f kg\n", peso);
        printf("Distancia Percorrida     : %.2f km\n", distancia);
        printf("Cupom Aplicado           : %.2f\n", cupom);
        printf("-----------------------------------------------------------\n");
        printf("Valor Bruto do Frete     : R$ %.2f\n", freteBruto);
        printf("Valor Final Calculado    : R$ %.2f\n", valorFinal);
        printf("===========================================================\n");
        printf("\nVoltando ao menu...\n");
        break;

    case 2:
        multiplicadorDistancia = 1.80 * distancia;
        freteBruto = (multiplicadorDistancia * distancia) + (peso * taxaDePeso) + 15;
        valorFinal = freteBruto - cupom;
        encomenda++;

        // Atualizando os acumuladores globais
        soma_peso += peso;
        soma_distancia += distancia;
        faturamento_total += valorFinal;

        printf("\n===========================================================\n");
        printf("               LOGTECH - COMPROVANTE DE FRETE              \n");
        printf("===========================================================\n");
        printf("Encomenda N              : %03d\n", encomenda);
        printf("Categoria Selecionada    : Expresso\n");
        printf("Peso Registrado          : %.2f kg\n", peso);
        printf("Distancia Percorrida     : %.2f km\n", distancia);
        printf("Cupom Aplicado           : %.2f\n", cupom);
        printf("-----------------------------------------------------------\n");
        printf("Valor Bruto do Frete     : R$ %.2f\n", freteBruto);
        printf("Valor Final Calculado    : R$ %.2f\n", valorFinal);
        printf("===========================================================\n");
        printf("\nVoltando ao menu...\n");
        break;

    case 3:
        multiplicadorDistancia = 2.50 * distancia;
        freteBruto = (multiplicadorDistancia * distancia) + (peso * taxaDePeso) + 30;
        valorFinal = freteBruto - cupom;
        encomenda++;

        // Atualizando os acumuladores globais
        soma_peso += peso;
        soma_distancia += distancia;
        faturamento_total += valorFinal;

        printf("\n===========================================================\n");
        printf("               LOGTECH - COMPROVANTE DE FRETE              \n");
        printf("===========================================================\n");
        printf("Encomenda N              : %03d\n", encomenda);
        printf("Categoria Selecionada    : Prioritaria\n");
        printf("Peso Registrado          : %.2f kg\n", peso);
        printf("Distancia Percorrida     : %.2f km\n", distancia);
        printf("Cupom Aplicado           : %.2f\n", cupom);
        printf("-----------------------------------------------------------\n");
        printf("Valor Bruto do Frete     : R$ %.2f\n", freteBruto);
        printf("Valor Final Calculado    : R$ %.2f\n", valorFinal);
        printf("===========================================================\n");
        printf("\nVoltando ao menu...\n");
        break;

    default:
        printf("Categoria não encontrada!\n");
        break;
    }

    return multiplicadorDistancia;
}

int registro(float somaPeso, int encomenda, float somaDistancia, float freteTotal)
{
    float mediaPeso = 0.0;
    float mediaDistancia = 0.0;
    
    // Previne divisão por zero caso nenhuma encomenda tenha sido cadastrada ainda
    if (encomenda > 0) {
        mediaPeso = somaPeso / encomenda;
        mediaDistancia = somaDistancia / encomenda;
    } else {
        printf("\n!!!!Valor de encomendas insuficiente\n\n");
        return 0;
    }

    // Impressão da tela
    printf("\n=========================================\n");
    printf("     RESUMO FINANCEIRO E OPERACIONAL     \n");
    printf("=========================================\n");
    printf("Total de encomendas processadas : %d\n", encomenda);
    printf("Faturamento total em fretes     : R$ %.2f\n", freteTotal);
    printf("Média de peso das encomendas   : %.2f kg\n", mediaPeso);
    printf("Média de distância percorrida   : %.2f km\n", mediaDistancia);
    printf("-----------------------------------------\n");
    printf("Maior frete individual cobrado : R$ %.2f\n", maior_frete);
    printf("Menor frete individual cobrado : R$ %.2f\n", menor_frete);
    printf("Total de isenções (Frete Grátis): %d encomendas\n", qtd_isencoes);
    printf("=========================================\n\n");

    return 0;
}