#include <stdio.h>
#include <string.h>
#include "grafo.h"
#include <ctype.h>
#include <stdlib.h> // Usar atoi

/* Giovana - Valida o nome da cidade

   - Deve conter pelo menos uma letra
   - Pode conter apenas letras e espaços
   - Não aceita números ou caracteres especiais
*/
int cidade_valida(const char *cidade) {
    int tem_letra = 0; // Serve para garantir que existe pelo menos uma letra

    // Analisa caractere por caractere
    while (*cidade) {
        if (isalpha((unsigned char)*cidade)) {
            tem_letra = 1;
        }
        // Se não for letra e não for espaço:
        else if (*cidade != ' ') {
            return 0; // Se não tem letra, já retorna 0
        }

        cidade++; // O ponteiro avança
    }

    return tem_letra; // Válido.
}

/* Giovana - Valida a sigla do aeroporto

   - Deve possuir exatamente 3 caracteres
   - Todos os caracteres devem ser letras
   - Não aceita números, espaços ou símbolos
*/
int sigla_valida(const char *sigla) {
    // Só pode haver 3 letras
    if (strlen(sigla) != 3) {
        return 0; // Inválido
    }

    // Percorre os caracteres
    for (int i = 0; sigla[i] != '\0'; i++) {

        // Verifica se cada caractere é letra
        if (!isalpha((unsigned char)sigla[i])) {
            return 0; // Inválido
        }
    }

    return 1; // Válido
}

/* Giovana - Converte texto para maiúsculo

   Padronizei as siglas dos aeroportos para melhorar as buscas e inserções
*/
void converter_para_maiusculo(char *texto) {
    // Percorre a string
    for (int i = 0; texto[i] != '\0'; i++) {

        // Converte cada letra
        texto[i] = toupper((unsigned char)texto[i]);
    }
}

/* 
   Giovana - Valida número do voo

   - Deve conter apenas dígitos
   - Deve possuir entre 1 e 4 dígitos
   - Não aceita letras, símbolos ou espaços
*/
int numero_voo_valido(const char *texto) {

    // Está vazio?
    if (strlen(texto) == 0) {
        return 0;
    }

    // Maior do que 4 caracteres?
    if (strlen(texto) > 4) {
        return 0;
    }

    // Percorre cada caractere e verifica se é digito
    for (int i = 0; texto[i] != '\0'; i++) {
        if (!isdigit((unsigned char)texto[i])) {
            return 0;
        }
    }

    return 1; // Valida se todos forem números
}

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


// Giovana - Cadastrar um novo aeroporto
void cadastrar_aeroporto(VetorAeroportos* vetor, MatrizEsparsa* matriz) {
    char cidade[50];
    char sigla[5];

    printf("Nome da cidade: ");
    scanf(" %49[^\n]", cidade);

    if (!cidade_valida(cidade)) {
        printf("Inválido! O nome da cidade deve conter apenas letras e espacos.\n");
        return;
    }

    printf("Sigla do aeroporto (ex: GRU): ");
    scanf(" %4s", sigla);

    if (!sigla_valida(sigla)) {
        printf("Inválido! A sigla deve conter exatamente 3 letras.\n");
        return;
    }

    converter_para_maiusculo(sigla);

    /* 
       Verificar duplicidade
       Essa função percorre todos os aeroportos, comparando as siglas
       Se encontrar -> Ja cadastrado
       Se não encontrar -> -1 
    */
    if (buscar_id_por_sigla(vetor, sigla) != -1) {
        printf("Inválido! Aeroporto '%s' ja esta cadastrado.\n", sigla);
        return;
    }

    // Se não tiver cadastrado, eu chamo a função para inserir
    inserir_aeroporto(vetor, cidade, sigla); // Copia para a próxima posição do vetor

    // Aumentar a matriz se estiver cheia
    if (vetor->tamanho > matriz->capacidade) {
        reajustar_matriz(matriz, vetor->tamanho);
    }

    printf("Aeroporto %s (%s) cadastrado com sucesso!\n",
           cidade, sigla);
}

// Giovana - Cadastrar um voo entre dois aeroportos
void cadastrar_voo(VetorAeroportos* vetor, MatrizEsparsa* matriz) {
    char sigla_origem[4];
    char sigla_destino[4];
    char entrada_voo[20];
    int numero_voo;

    // Lê as siglas de origem + Validação
    printf("Sigla do aeroporto de origem: ");
    scanf(" %3s", sigla_origem);

    if (!sigla_valida(sigla_origem)) {
        printf("Inválido. A sigla de origem deve conter exatamente 3 letras.\n");
        return;
    }

    // Lê as siglas de destino + Validação
    printf("Sigla do aeroporto de destino: ");
    scanf(" %3s", sigla_destino);

    if (!sigla_valida(sigla_destino)) {
        printf("Inválido! A sigla de destino deve conter exatamente 3 letras.\n");
        return;
    }

    converter_para_maiusculo(sigla_origem);
    converter_para_maiusculo(sigla_destino);

    // Lê o número do voo.
    printf("Numero do voo: ");
    scanf(" %19s", entrada_voo);

    if (!numero_voo_valido(entrada_voo)) {
        printf("Inválido!. O numero do voo deve conter apenas digitos e ter no maximo 4 digitos.\n");
        return;
    }

    // Aprendi que o ATOI converte texto para inteiro
    numero_voo = atoi(entrada_voo);

    if (numero_voo <= 0) {
        printf("Número do voo inválido.\n");
        return;
    }

    // Localizar aeroportos pelo ID
    // Ex: GRU, retorna "3"
    int id_origem = buscar_id_por_sigla(vetor, sigla_origem);
    int id_destino = buscar_id_por_sigla(vetor, sigla_destino);

    // Validar existência
    if (id_origem == -1) { // Se -1 -> Sigla não existe
        printf("Inválido! Aeroporto de origem '%s' nao encontrado.\n",
               sigla_origem);
        return;
    }

    if (id_destino == -1) {
        printf("Inválido! Aeroporto de destino '%s' nao encontrado.\n",
               sigla_destino);
        return;
    }

    if (id_origem == id_destino) {
        printf("Inválido! Origem e destino nao podem ser o mesmo aeroporto.\n");
        return;
    }

    // Verificar se já existe, procura na matriz
    // Se não existir, tem que voltar o valor padrão = "-1"
    if (obter_distancia(matriz, id_origem, id_destino) != matriz->valor_padrao) {
        printf("Inválido! Já existe um voo cadastrado de %s para %s.\n",
               sigla_origem, sigla_destino);
        return;
    }

    // Guarda o voo na matriz
    guardar_voo(matriz, id_origem, id_destino, numero_voo);

    printf("Voo %03d de %s para %s cadastrado com sucesso!\n",
           numero_voo, sigla_origem, sigla_destino);
}

// Giovana - Remover um voo pelo número
void remover_voo_por_numero(VetorAeroportos* vetor, MatrizEsparsa* matriz) {
    char entrada_voo[20];
    int numero_voo;

    // ler entrada
    printf("Numero do voo a remover: ");
    scanf(" %19s", entrada_voo);

    if (!numero_voo_valido(entrada_voo)) {
        printf("Inválido! O número do voo deve conter apenas digitos e ter no maximo 4 digitos.\n");
        return;
    }
    
    // Converter para inteiro
    numero_voo = atoi(entrada_voo);

    // Verificar se é positivo
    if (numero_voo <= 0) {
        printf("Inválido! Número do voo inválido.\n");
        return;
    }

    /* 
       0 = Ainda não encontrei o voo
       1 = Encontrei
     */
    int encontrado = 0;

    // Percorre todas as linhas da matriz, o FOR visita cada linha
    for (int i = 0; i < matriz->capacidade; i++) {
        NoMatriz* atual = matriz->voos_linha[i];

        // Percorrer a lista daquela linha
        while (atual != NULL) {

            // Comparar o número do voo
            if (atual->distancia == numero_voo) {

                //(matriz, linha, coluna)
                remover_voo(matriz, i, atual->coluna);

                printf("Voo %03d removido com sucesso!\n",
                       numero_voo);

                // Encontrado -> Sai do while
                encontrado = 1;
                break;
            }

            atual = atual->proxima_linha;
        }

        // Sai do for
        if (encontrado) {
            break;
        }
    }

    if (!encontrado) {
        printf("Inválido! Voo %03d nao encontrado.\n",
               numero_voo);
    }
}
