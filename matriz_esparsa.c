#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "matriz_esparsa.h"


bool inicializar_matriz(Matriz_esparsa* m, int capacidade, int valor_padrao) {
    if(m == NULL) return false;
    //aloca memoria ara os ponteiros
    m->capacidade = capacidade;
    m->aero = (Aeroporto*) malloc(capacidade * sizeof(Aeroporto));
    m->voos_linha = (NoMatriz**) malloc(capacidade * sizeof(NoMatriz*));
    m->voos_coluna = (NoMatriz**) malloc(capacidade * sizeof(NoMatriz*)); 

    // zera as variáveis
    for(int i = 0; i < capacidade; i++) {
        m->voos_linha[i] = NULL;
        m->voos_coluna[i] = NULL;
    }

    m->tamanho = 0;
    m->valor_padrao = valor_padrao;
    return true;
}

bool inserir_aeroporto(Matriz_esparsa* m, char* cidade, char* sigla) {
    if(m == NULL) return false;
    
    //realocar a capacidade do vetor e seus ponteiros
    if(m->tamanho == m->capacidade) { 
    m->capacidade = m->capacidade * 2;
    m->aero = (Aeroporto*) realloc(m->aero, m->capacidade * sizeof(Aeroporto));
    m->voos_linha = (NoMatriz**) realloc(m->voos_linha, m->capacidade * sizeof(NoMatriz*));
    m->voos_coluna = (NoMatriz**) realloc(m->voos_coluna, m->capacidade * sizeof(NoMatriz*)); 
    
    // Zera os novos ponteiros 
        for(int i = m->tamanho; i < m->capacidade; i++) {
            m->voos_linha[i] = NULL;
            m->voos_coluna[i] = NULL;
        }
    }

    //atribuindo os valores dos parâmetros desse novo aeroporto
    strcpy(m->aero[m->tamanho].cidade, cidade);
    strcpy(m->aero[m->tamanho].sigla, sigla);
    m->aero[m->tamanho].id = m->tamanho;
    m->tamanho++;
    
    return true;
}

bool guardar_voo(Matriz_esparsa* m, int linha, int coluna, int distancia) {
    if(m == NULL) return false;

    // Criando nodos de apoio
    NoMatriz* anterior = NULL;
    NoMatriz* atual = m->voos_linha[linha];

    //Procurar voo na matriz
    while(atual != NULL && atual->coluna < coluna){
        anterior = atual;
        atual = atual->proxima_linha;
    }

    // Armazenamento de acordo com as condições

    // Condição 1: Voo não existe e valor é diferente do padrão
    if(atual == NULL || atual->coluna != coluna){
        if(distancia != m->valor_padrao) {
            NoMatriz* novo_voo = (NoMatriz*) malloc(sizeof(NoMatriz));
            if(novo_voo == NULL) return false;

            novo_voo->linha = linha;
            novo_voo->coluna = coluna;
            novo_voo->distancia = distancia;
            
            // Insere no início da Lista da Linha
            novo_voo->proxima_linha = m->voos_linha[linha];
            m->voos_linha[linha] = novo_voo;

            // Insere no início da Lista da Coluna (crucial para a matriz ortogonal)
            novo_voo->proxima_coluna = m->voos_coluna[coluna];
            m->voos_coluna[coluna] = novo_voo;
            }
        } else if(distancia != m->valor_padrao){
            // Condição 2: Voo existe e valor é diferente do padrão
            atual->distancia = distancia;
        } else {
            // Condição 3: Voo existe e é igual ao padrão(remoção)
            if(anterior == NULL) {
            m->voos_linha[linha] = atual->proxima_linha;
        } else {
            anterior->proxima_linha = atual->proxima_linha;
        }
            free(atual);
    }
    return true;
}

int obter_distancia(Matriz_esparsa* m, int linha, int coluna){
    NoMatriz* anterior = NULL;
    NoMatriz* atual = m->voos_linha[linha];

    // Busca pela distancia na matriz
    while(atual != NULL && atual->coluna < coluna) {
        anterior = atual;
        atual = atual->proxima_linha;
    } // Caso exista retorna o valor
    if(atual != NULL && atual->coluna == coluna){
        return atual->distancia;
    } else{ // Caso não exista retorna o valor_padrão
        return m->valor_padrao;
    }
}

bool remover_voo(Matriz_esparsa* m, int linha, int coluna){
    if(m == NULL) return false;

    // Criando nodos de apoio
    NoMatriz* anterior = NULL;
    NoMatriz* atual = m->voos_linha[linha];

    //Procurando voo na matriz
    while(atual != NULL && atual->coluna < coluna){
        anterior = atual;
        atual = atual->proxima_linha;
    }
    //Ajusta os ponteiros e libera o nó
    if(atual != NULL && atual->coluna == coluna){
        if(anterior == NULL) {
                m->voos_linha[linha] = atual->proxima_linha;
            } else {
                anterior->proxima_linha = atual->proxima_linha;
            }
            free(atual);
            return true;
            }
        return false;
}

void cronograma_de_voos(Matriz_esparsa* m, char* cidade) {
    if(m == NULL) return;

    int id_origem = -1;
    for(int i = 0; i < m->tamanho; i++) {
        if(strcmp(m->aero[i].cidade, cidade) == 0) {
            id_origem = m->aero[i].id;
            break;
        }
    }

    if(id_origem == -1) {
        printf("Aeroporto nao encontrado.\n");
        return;
    }

    NoMatriz* atual = m->voos_linha[id_origem];
    while(atual != NULL) {
        printf("Voo de %s para %s (ID: %d) | Distancia: %d km\n", cidade, m->aero[atual->coluna].cidade, atual->coluna, atual->distancia);
        atual = atual->proxima_linha;
    }
}

int buscar_id_por_sigla(Matriz_esparsa* m, char* sigla) {
    for(int i = 0; i < m->tamanho; i++) {
        if(strcmp(m->aero[i].sigla, sigla) == 0) {
            return m->aero[i].id;
        }
    }
    return -1;
}

void liberar_matriz(Matriz_esparsa* m) {
    if(m == NULL) return;

    for(int i = 0; i < m->tamanho; i++) {
        NoMatriz* atual = m->voos_linha[i]; 
        while(atual != NULL) {
            NoMatriz* proximo = atual->proxima_linha;
            free(atual);
            atual = proximo;
        }
    }

    if(m->voos_linha != NULL) free(m->voos_linha);
    if(m->voos_coluna != NULL) free(m->voos_coluna);

    if(m->aero != NULL) {
        free(m->aero);
    }

    m->tamanho = 0;
    m->capacidade = 0;
}