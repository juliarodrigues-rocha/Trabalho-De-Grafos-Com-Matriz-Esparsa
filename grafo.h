#ifndef _GRAFO_H_
#define _GRAFO_H_

#include "aeroporto.h"
#include "matriz_esparsa.h"

/* Cadastra um novo aeroporto no vetor dinâmico e expande
   a matriz esparsa para comportar a nova linha/coluna.
   Verifica se a sigla já existe antes de inserir.
 */
void cadastrar_aeroporto(VetorAeroportos* vetor, MatrizEsparsa* matriz);

/* Cadastra um voo (aresta) entre dois aeroportos identificados
   pelas suas siglas. O número do voo é guardado no campo
   "distancia" do nó da matriz esparsa.
   Verifica consistência: aeroportos existem, não são iguais,
   número válido e rota ainda sem voo cadastrado.
 */
void cadastrar_voo(VetorAeroportos* vetor, MatrizEsparsa* matriz);

/* Remove um voo da matriz esparsa buscando pelo número do voo.
   Percorre todas as linhas da matriz até encontrar a aresta
   cujo campo "distancia" corresponde ao número informado.
 */
void remover_voo_por_numero(VetorAeroportos* vetor, MatrizEsparsa* matriz);

/*
 * Função 4:
 * Lista todos os voos que partem de um determinado aeroporto.
 * Mostra o número do voo e o nome da cidade de destino.
 *
 * Parâmetros:
 *   - matriz: a matriz esparsa com os voos cadastrados
 *   - vetor: o vetor dinâmico com os aeroportos
 *   - sigla_origem: sigla do aeroporto de partida (ex: "GRU")
 */
void listar_voos_de_aeroporto(MatrizEsparsa* matriz, VetorAeroportos* vetor, char* sigla_origem);

/*
 * Função 5:
 * Lista todos os trajetos possíveis entre dois aeroportos,
 * incluindo voos diretos e com escalas.
 * Usa busca em profundidade (DFS) para encontrar os caminhos.
 *
 * Parâmetros:
 *   - matriz: a matriz esparsa com os voos cadastrados
 *   - vetor: o vetor dinâmico com os aeroportos
 *   - sigla_origem: sigla do aeroporto de partida
 *   - sigla_destino: sigla do aeroporto de chegada
 */
void listar_trajetos(MatrizEsparsa* matriz, VetorAeroportos* vetor, char* sigla_origem, char* sigla_destino);

#endif