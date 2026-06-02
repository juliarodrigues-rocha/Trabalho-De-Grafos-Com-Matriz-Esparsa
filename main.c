#include <stdio.h>
#include <string.h>
#include "aeroporto.h"
#include "matriz_esparsa.h"
#include "grafo.h"

// Limpa o buffer do teclado para evitar leituras erradas
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Exibe o menu principal na tela
void exibir_menu() {
    printf("\n==========================================\n");
    printf("     SISTEMA DE MALHA AEREA - ANAC\n");
    printf("==========================================\n");
    printf("  1. Cadastrar aeroporto\n");
    printf("  2. Cadastrar voo entre aeroportos\n");
    printf("  3. Remover voo\n");
    printf("  4. Listar voos saindo de um aeroporto\n");
    printf("  5. Listar trajetos entre dois aeroportos\n");
    printf("  0. Sair\n");
    printf("------------------------------------------\n");
    printf("Escolha uma opcao: ");
}

// ==========================================
// Função principal com o menu interativo
// ==========================================
int main() {
    VetorAeroportos vetor;
    MatrizEsparsa matriz;

    // Começa com capacidade 5 (os aeroportos do enunciado)
    inicializar_vetor(&vetor, 5);
    inicializar_matriz(&matriz, 5, -1);  // -1 significa "sem voo"

    // Pré-carrega os aeroportos do enunciado para facilitar os testes
    inserir_aeroporto(&vetor, "Brasilia",       "BSB");
    inserir_aeroporto(&vetor, "Belo Horizonte", "CNF");
    inserir_aeroporto(&vetor, "Rio de Janeiro", "GIG");
    inserir_aeroporto(&vetor, "Sao Paulo",      "GRU");
    inserir_aeroporto(&vetor, "Salvador",       "SSA");

    // Pré-carrega os voos do diagrama do enunciado
    guardar_voo(&matriz, 0, 4, 107);  // BSB -> SSA: voo 107
    guardar_voo(&matriz, 1, 2, 555);  // CNF -> GIG: voo 555
    guardar_voo(&matriz, 1, 3, 101);  // CNF -> GRU: voo 101
    guardar_voo(&matriz, 1, 4, 214);  // CNF -> SSA: voo 214
    guardar_voo(&matriz, 2, 1, 554);  // GIG -> CNF: voo 554
    guardar_voo(&matriz, 2, 3, 90);   // GIG -> GRU: voo 090
    guardar_voo(&matriz, 3, 0, 50);   // GRU -> BSB: voo 050
    guardar_voo(&matriz, 3, 1, 102);  // GRU -> CNF: voo 102
    guardar_voo(&matriz, 3, 2, 89);   // GRU -> GIG: voo 089
    guardar_voo(&matriz, 4, 1, 215);  // SSA -> CNF: voo 215

    printf("Sistema iniciado com os aeroportos e voos do enunciado.\n");

    int opcao;

    do {
        exibir_menu();
        scanf("%d", &opcao);
        limpar_buffer();

        switch (opcao) {
            case 1:
                cadastrar_aeroporto(&vetor, &matriz);
                break;

            case 2:
                cadastrar_voo(&vetor, &matriz);
                break;

            case 3:
                remover_voo_por_numero(&vetor, &matriz);
                break;

            case 4: {
                char sigla[5];
                printf("Sigla do aeroporto de origem: ");
                scanf(" %4s", sigla);
                listar_voos_de_aeroporto(&matriz, &vetor, sigla);
                break;
            }

            case 5: {
                char sigla_origem[5];
                char sigla_destino[5];
                printf("Sigla do aeroporto de origem: ");
                scanf(" %4s", sigla_origem);
                printf("Sigla do aeroporto de destino: ");
                scanf(" %4s", sigla_destino);
                listar_trajetos(&matriz, &vetor, sigla_origem, sigla_destino);
                break;
            }

            case 0:
                printf("Encerrando o sistema...\n");
                break;

            default:
                printf("Opcao invalida. Tente novamente.\n");
        }

    } while (opcao != 0);

    // Libera a memória alocada antes de encerrar
    liberar_vetor(&vetor);
    liberar_matriz(&matriz);

    return 0;
}
