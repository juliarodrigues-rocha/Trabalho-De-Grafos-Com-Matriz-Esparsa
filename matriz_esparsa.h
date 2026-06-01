#ifndef _MATRIZ_ESPARSA_H_
#define _MATRIZ_ESPARSA_H_

#include <stdbool.h>

typedef struct NoMatriz {
    int linha; //Id origem
    int coluna; //Id destino
    int distancia; 
    struct NoMatriz* proxima_linha; //Próximo aeroporto de origem
} NoMatriz;

typedef struct {
    NoMatriz** voos_linha; 
    int capacidade;
    int valor_padrao;
} MatrizEsparsa;

bool inicializar_matriz(MatrizEsparsa* m, int capacidade, int valor_padrao);
bool reajustar_matriz(MatrizEsparsa* m, int nova_capacidade);
bool guardar_voo(MatrizEsparsa* m, int linha, int coluna, int distancia);
int obter_distancia(MatrizEsparsa* m, int linha, int coluna);
bool remover_voo(MatrizEsparsa* m, int linha, int coluna);
void liberar_matriz(MatrizEsparsa* m);

#endif