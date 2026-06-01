#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "aeroporto.h"

bool inicializar_vetor(VetorAeroportos* v, int capacidade) {
    if (v == NULL) return false;
    //aloca capacidade para o vetor e zera seus campos
    v->capacidade = capacidade;
    v->aero = (Aeroporto*) malloc(capacidade * sizeof(Aeroporto));
    v->tamanho = 0;
    return true;
}

bool inserir_aeroporto(VetorAeroportos* v, char* cidade, char* sigla) {
    if (v == NULL) return false;
    //realoca a capacidade para seu dobro
    if (v->tamanho == v->capacidade) {
        v->capacidade = v->capacidade * 2;
        v->aero = (Aeroporto*) realloc(v->aero, v->capacidade * sizeof(Aeroporto));
    }

    //Insere a cidade, sigla e tamanho do aeroporto
    strcpy(v->aero[v->tamanho].cidade, cidade);
    strcpy(v->aero[v->tamanho].sigla, sigla);
    v->aero[v->tamanho].id = v->tamanho;
    v->tamanho++;

    return true;
}

int buscar_id_por_sigla(VetorAeroportos* v, char* sigla) {
    if (v == NULL) return false;

    // busca a sigla da cidade no vetor de aeroportos
    for (int i = 0; i < v->tamanho; i++) {
        if (strcmp(v->aero[i].sigla, sigla) == 0) {
            return v->aero[i].id;
        }
    }
    return -1;
}

void liberar_vetor(VetorAeroportos* v) {
    if (v == NULL) return;

    if (v->aero != NULL) {
        free(v->aero);
    }
    v->tamanho = 0;
    v->capacidade = 0;
}