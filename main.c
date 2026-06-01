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

// Opção 1: cadastra um novo aeroporto no vetor e expande a matriz se necessário
void opcao_cadastrar_aeroporto(VetorAeroportos* vetor, MatrizEsparsa* matriz) {
    char cidade[50];
    char sigla[5];

    printf("Nome da cidade: ");
    scanf(" %49[^\n]", cidade);
    printf("Sigla do aeroporto (ex: GRU): ");
    scanf(" %4s", sigla);

    // Verifica se a sigla já existe para evitar duplicata
    if (buscar_id_por_sigla(vetor, sigla) != -1) {
        printf("Erro: aeroporto '%s' ja esta cadastrado.\n", sigla);
        return;
    }

    inserir_aeroporto(vetor, cidade, sigla);

    // A matriz precisa crescer junto com o vetor de aeroportos
    // (uma linha/coluna nova para o aeroporto recém-cadastrado)
    if (vetor->tamanho > matriz->capacidade) {
        reajustar_matriz(matriz, vetor->tamanho);
    }

    printf("Aeroporto %s (%s) cadastrado com sucesso!\n", cidade, sigla);
}

// Opção 2: cadastra um voo (aresta) entre dois aeroportos
void opcao_cadastrar_voo(VetorAeroportos* vetor, MatrizEsparsa* matriz) {
    char sigla_origem[5];
    char sigla_destino[5];
    int numero_voo;

    printf("Sigla do aeroporto de origem: ");
    scanf(" %4s", sigla_origem);
    printf("Sigla do aeroporto de destino: ");
    scanf(" %4s", sigla_destino);
    printf("Numero do voo: ");
    scanf("%d", &numero_voo);

    int id_origem  = buscar_id_por_sigla(vetor, sigla_origem);
    int id_destino = buscar_id_por_sigla(vetor, sigla_destino);

    if (id_origem == -1) {
        printf("Erro: aeroporto de origem '%s' nao encontrado.\n", sigla_origem);
        return;
    }
    if (id_destino == -1) {
        printf("Erro: aeroporto de destino '%s' nao encontrado.\n", sigla_destino);
        return;
    }
    if (id_origem == id_destino) {
        printf("Erro: origem e destino nao podem ser o mesmo aeroporto.\n");
        return;
    }
    if (numero_voo <= 0) {
        printf("Erro: numero do voo invalido.\n");
        return;
    }

    // Verifica se já existe um voo cadastrado nessa rota
    if (obter_distancia(matriz, id_origem, id_destino) != matriz->valor_padrao) {
        printf("Erro: ja existe um voo cadastrado de %s para %s.\n",
               sigla_origem, sigla_destino);
        return;
    }

    // Na matriz esparsa, usamos o campo "distancia" para guardar o número do voo
    guardar_voo(matriz, id_origem, id_destino, numero_voo);
    printf("Voo %03d de %s para %s cadastrado com sucesso!\n",
           numero_voo, sigla_origem, sigla_destino);
}

// Opção 3: remove um voo indicado pelo número
void opcao_remover_voo(VetorAeroportos* vetor, MatrizEsparsa* matriz) {
    int numero_voo;

    printf("Numero do voo a remover: ");
    scanf("%d", &numero_voo);

    // Precisa procurar na matriz qual aresta tem esse número de voo
    int encontrado = 0;

    for (int i = 0; i < matriz->capacidade; i++) {
        NoMatriz* atual = matriz->voos_linha[i];

        while (atual != NULL) {
            if (atual->distancia == numero_voo) {
                // Encontrou o voo: remove a aresta da matriz
                remover_voo(matriz, i, atual->coluna);
                printf("Voo %03d removido com sucesso!\n", numero_voo);
                encontrado = 1;
                break;
            }
            atual = atual->proxima_linha;
        }

        if (encontrado) break;
    }

    if (!encontrado) {
        printf("Erro: voo %03d nao encontrado.\n", numero_voo);
    }
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
                opcao_cadastrar_aeroporto(&vetor, &matriz);
                break;

            case 2:
                opcao_cadastrar_voo(&vetor, &matriz);
                break;

            case 3:
                opcao_remover_voo(&vetor, &matriz);
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