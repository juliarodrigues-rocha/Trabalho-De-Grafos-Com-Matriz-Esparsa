#ifndef _AEROPORTO_H_
#define _AEROPORTO_H_

#include <stdbool.h>

typedef struct {
    char cidade[50];
    char sigla[5];
    int id;
} Aeroporto;

typedef struct {
    Aeroporto* aero; //ponteiro para a struct de aeroporto
    int capacidade;
    int tamanho;
} VetorAeroportos;

bool inicializar_vetor(VetorAeroportos* v, int capacidade);
bool inserir_aeroporto(VetorAeroportos* v, char* cidade, char* sigla);
int buscar_id_por_sigla(VetorAeroportos* v, char* sigla);
void liberar_vetor(VetorAeroportos* v);

#endif