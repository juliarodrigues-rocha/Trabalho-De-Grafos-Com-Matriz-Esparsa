#include <stdio.h>
#include <string.h>
#include "grafo.h"


//                   JÚLIA - 31/05/2026
/* =========================================================
 *    FUNÇÃO 4 - Listar voos que partem de um aeroporto
 * =========================================================
 *
 * A ideia é simples: dado o aeroporto de origem, buscamos
 * a linha dele na matriz esparsa. Cada nó existente nessa
 * linha representa um voo cadastrado saindo dali.
 *
 * Na matriz esparsa, cada linha corresponde a um aeroporto
 * de origem (identificado pelo seu id), e cada nó da lista
 * encadeada nessa linha é um destino com um número de voo.
 */
void listar_voos_de_aeroporto(MatrizEsparsa* matriz, VetorAeroportos* vetor, char* sigla_origem) {

    // Busca o id do aeroporto de origem pela sigla
    int id_origem = buscar_id_por_sigla(vetor, sigla_origem);

    if (id_origem == -1) {
        printf("Aeroporto '%s' nao encontrado.\n", sigla_origem);
        return;
    }

    // Garante que o id está dentro dos limites da matriz
    if (id_origem >= matriz->capacidade) {
        printf("Nenhum voo cadastrado saindo de %s.\n", sigla_origem);
        return;
    }

    // Percorre a lista encadeada da linha do aeroporto de origem
    NoMatriz* atual = matriz -> voos_linha[id_origem];

    if (atual == NULL) {
        printf("Nenhum voo cadastrado saindo de %s (%s).\n",
               vetor -> aero[id_origem].cidade, sigla_origem);
        return;
    }

    printf("\nVoos saindo de %s (%s):\n", vetor->aero[id_origem].cidade, sigla_origem);
    printf("------------------------------------------\n");

    // Cada nó da lista é um voo: a coluna é o id do destino, a distancia é o número do voo
    while (atual != NULL) {
        int id_destino = atual->coluna;
        int numero_voo = atual->distancia;

        printf("  Voo %03d -> %s (%s)\n",
               numero_voo,
               vetor->aero[id_destino].cidade,
               vetor->aero[id_destino].sigla);

        atual = atual->proxima_linha;
    }

    printf("------------------------------------------\n");
}



//                         JÚLIA - 31/05/2026
/* =========================================================================
 *     FUNÇÃO 5 - Listar todos os trajetos possíveis entre dois aeroportos
 * =========================================================================
 *
 * Aqui usamos DFS (busca em profundidade) para encontrar
 * todos os caminhos possíveis do aeroporto de origem até
 * o aeroporto de destino.
 *
 * A DFS funciona assim:
 *   - A partir da origem, visitamos cada aeroporto vizinho
 *   - Marcamos os aeroportos já visitados para não entrar em loop
 *   - Se chegamos ao destino, imprimimos o caminho percorrido
 *   - Depois voltamos (backtrack) e tentamos outros caminhos
 *
 * O vetor "caminho" guarda os ids dos aeroportos visitados
 * no trajeto atual, e "visitado" evita que passemos pelo
 * mesmo aeroporto duas vezes no mesmo trajeto.
 */

// Função auxiliar recursiva que faz a DFS de verdade
// - id_atual: aeroporto onde estamos agora
// - id_destino: aeroporto que queremos alcançar
// - visitado: array que marca quais aeroportos já foram visitados neste caminho
// - caminho: array que guarda os ids dos aeroportos visitados na ordem
// - tamanho_caminho: quantos aeroportos tem no caminho até agora
static void dfs(MatrizEsparsa* matriz, VetorAeroportos* vetor, int id_atual, int id_destino, int* visitado, int* caminho, int tamanho_caminho) {

    // Marca o aeroporto atual como visitado e adiciona ao caminho
    visitado[id_atual] = 1;
    caminho[tamanho_caminho] = id_atual;
    tamanho_caminho++;

    // Se chegamos ao destino, imprime o trajeto completo
    if (id_atual == id_destino) {
        printf("  ");
        for (int i = 0; i < tamanho_caminho; i++) {
            int id = caminho[i];
            printf("%s (%s)", vetor->aero[id].cidade, vetor->aero[id].sigla);

            // Coloca uma seta entre os aeroportos, exceto no último
            if (i < tamanho_caminho - 1) {
                printf(" -> ");
            }
        }
        printf("\n");

    } else {
        // Ainda não chegamos: visita cada vizinho do aeroporto atual
        // (ou seja, percorre a linha do id_atual na matriz esparsa)
        NoMatriz* voo = matriz->voos_linha[id_atual];

        while (voo != NULL) {
            int proximo = voo->coluna;

            // Só vai para o próximo se ainda não foi visitado neste caminho
            if (!visitado[proximo]) {
                dfs(matriz, vetor, proximo, id_destino, visitado, caminho, tamanho_caminho);
            }
            voo = voo->proxima_linha;
        }
    }
    // Backtrack: desmarca o aeroporto para liberar para outros caminhos
    visitado[id_atual] = 0;
}

// Função principal que o menu chama - prepara tudo e dispara a DFS
void listar_trajetos(MatrizEsparsa* matriz, VetorAeroportos* vetor, char* sigla_origem, char* sigla_destino) {

    int id_origem  = buscar_id_por_sigla(vetor, sigla_origem);
    int id_destino = buscar_id_por_sigla(vetor, sigla_destino);

    if (id_origem == -1) {
        printf("Aeroporto de origem '%s' nao encontrado.\n", sigla_origem);
        return;
    }
    if (id_destino == -1) {
        printf("Aeroporto de destino '%s' nao encontrado.\n", sigla_destino);
        return;
    }
    if (id_origem == id_destino) {
        printf("Origem e destino sao o mesmo aeroporto.\n");
        return;
    }

    // Aloca os arrays auxiliares com base no número de aeroportos cadastrados
    int n = vetor->tamanho;
    int visitado[n];
    int caminho[n];

    // Inicializa todos como não visitados
    for (int i = 0; i < n; i++) {
        visitado[i] = 0;
    }

    printf("\nTrajetos possiveis de %s (%s) ate %s (%s):\n",
           vetor->aero[id_origem].cidade,  sigla_origem,
           vetor->aero[id_destino].cidade, sigla_destino);
    printf("------------------------------------------\n");

    // Dispara a busca em profundidade a partir da origem
    dfs(matriz, vetor, id_origem, id_destino, visitado, caminho, 0);

    printf("------------------------------------------\n");
}