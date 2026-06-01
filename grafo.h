#ifndef _GRAFO_H_
#define _GRAFO_H_

#include "aeroporto.h"
#include "matriz_esparsa.h"

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