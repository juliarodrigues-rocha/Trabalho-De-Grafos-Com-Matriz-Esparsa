#ifndef _MATRIZ_ESPARSA_H_
#define _MATRIZ_ESPARSA_H_

#include <stdbool.h>

//Guarda as informações da vértice
typedef struct {
    char cidade[50];
    char sigla[5];
    int id;
} Aeroporto;

//Nó de cada voo
typedef struct NoMatriz {
    int linha;
    int coluna;
    int distancia;
    struct NoMatriz* proxima_linha;  
    struct NoMatriz* proxima_coluna;
} NoMatriz;

//Matriz por completo contendo capacidade e ponteiros
typedef struct {
    Aeroporto* aero; //aponta para o vetor dos aeroportos
    NoMatriz** voos_linha; //primeiro no da linha
    NoMatriz** voos_coluna; //primeiro no da coluna
    int capacidade;
    int tamanho;
    int valor_padrao;
} Matriz_esparsa;

bool inicializar_matriz(Matriz_esparsa* m, int capacidade, int valor_padrao);
bool inserir_aeroporto(Matriz_esparsa* m, char* cidade, char* sigla);
bool guardar_voo(Matriz_esparsa* m, int linha, int coluna, int distancia);
int obter_distancia(Matriz_esparsa* m, int linha, int coluna);
bool remover_voo(Matriz_esparsa* m, int linha, int coluna);
void cronograma_de_voos(Matriz_esparsa* m, char* cidade);
int buscar_id_por_sigla(Matriz_esparsa* m, char* sigla);
void liberar_matriz(Matriz_esparsa* m);

#endif