#include <stdio.h>
#include <stdlib.h>
#include "matriz_esparsa.h"

bool inicializar_matriz(MatrizEsparsa* m, int capacidade, int valor_padrao) {
    if (m == NULL) return false;
    //aloca capacidade para o vetor e a matriz
    m->capacidade = capacidade;
    m->voos_linha = (NoMatriz**) malloc(capacidade * sizeof(NoMatriz*));

    //deixa o
    for (int i = 0; i < capacidade; i++) {
        m->voos_linha[i] = NULL;
    }

    m->valor_padrao = valor_padrao;
    return true;
}

bool reajustar_matriz(MatrizEsparsa* m, int nova_capacidade) {
    if (m == NULL || nova_capacidade <= m->capacidade) return false;
    //aloca uma nova capacidade para a matriz
    m->voos_linha = (NoMatriz**) realloc(m->voos_linha, nova_capacidade * sizeof(NoMatriz*));

    for (int i = m->capacidade; i < nova_capacidade; i++) {
        m->voos_linha[i] = NULL;
    }

    m->capacidade = nova_capacidade;
    return true;
}

bool guardar_voo(MatrizEsparsa* m, int linha, int coluna, int distancia) {
    if (m == NULL) return false;

    NoMatriz* anterior = NULL;
    NoMatriz* atual = m->voos_linha[linha];

    while (atual != NULL && atual->coluna < coluna) {
        anterior = atual;
        atual = atual->proxima_linha;
    }
    // Condição 1: valor é nulo e diferente do valor padrão
    if (atual == NULL || atual->coluna != coluna) {
        if (distancia != m->valor_padrao) {
            NoMatriz* novo_voo = (NoMatriz*) malloc(sizeof(NoMatriz));
            if (novo_voo == NULL) return false;

            novo_voo->linha = linha;
            novo_voo->coluna = coluna;
            novo_voo->distancia = distancia;

            /* Correção feita por Giovana: encadeia respeitando a posição ordenada
            Antes: Sempre insere o novo voo no começo da lista
            Agora: Insere exatamente onde o while determinou, mantendo a ordenação por coluna
            */
            novo_voo->proxima_linha = atual;
            if (anterior == NULL) {
                m->voos_linha[linha] = novo_voo;
            } else {
                anterior->proxima_linha = novo_voo;
            }
        }
        // Condição 2: valor existe e é diferente do valor padrão
    } else if (distancia != m->valor_padrao) {
        atual->distancia = distancia;
    } else {
        // Condição 3: valor existe e e igual ao padrao
        if (anterior == NULL) {
            m->voos_linha[linha] = atual->proxima_linha;
        } else {
            anterior->proxima_linha = atual->proxima_linha;
        }
        free(atual);
    }
    return true;
}

int obter_distancia(MatrizEsparsa* m, int linha, int coluna) {
    if (m == NULL) return 0;
    NoMatriz* atual = m->voos_linha[linha];

    // percorre a lista ate achar o voo daquela distancia
    while (atual != NULL && atual->coluna < coluna) {
        atual = atual->proxima_linha;
    } 
    if (atual != NULL && atual->coluna == coluna) {
        return atual->distancia;
    } else { 
        return m->valor_padrao;
    }
}

bool remover_voo(MatrizEsparsa* m, int linha, int coluna) {
    if (m == NULL) return false;

    NoMatriz* anterior = NULL;
    NoMatriz* atual = m->voos_linha[linha];

    //percorre a lista ate encontrar o voo a ser removido
    while (atual != NULL && atual->coluna < coluna) {
        anterior = atual;
        atual = atual->proxima_linha;
    }
    
    if (atual != NULL && atual->coluna == coluna) {
        if (anterior == NULL) {
            m->voos_linha[linha] = atual->proxima_linha;
        } else {
            anterior->proxima_linha = atual->proxima_linha;
        }
        free(atual);
        return true;
    }
    return false;
}

void liberar_matriz(MatrizEsparsa* m) {
    if (m == NULL) return;
    //para cada elemento da matriz ocorre o free
    for (int i = 0; i < m->capacidade; i++) {
        NoMatriz* atual = m->voos_linha[i]; 
        while (atual != NULL) {
            NoMatriz* proximo = atual->proxima_linha;
            free(atual);
            atual = proximo;
        }
    }

    //libera o ponteiro com NULL
    if (m->voos_linha != NULL) free(m->voos_linha);

    m->capacidade = 0;
}
