#include "../../include/logica.h"
#include <stdlib.h>
#include <string.h>

// Função que cria uma cópia de uma pilha
void copiar_pilha(Pilha *dest, Pilha *orig) {
    int i = 0;
    strcpy(dest->nome_tipo, orig->nome_tipo);
    dest->quantidade = orig->quantidade;
    if (orig->quantidade > 0) {
        dest->cartas = malloc(orig->quantidade * sizeof(Carta));
        while (i < orig->quantidade) {
            dest->cartas[i] = orig->cartas[i];
            i++;
        }
    } else {
        dest->cartas = NULL;
    }
}

// Função que guarda o estado atual
EstadoJogo* guardar_estado(EstadoJogo *atual) {
    EstadoJogo *novo = malloc(sizeof(EstadoJogo));
    int i = 0;
    novo->total_pilhas = atual->total_pilhas;
    novo->pilhas = malloc(atual->total_pilhas * sizeof(Pilha));
    
    while (i < atual->total_pilhas) {
        copiar_pilha(&novo->pilhas[i], &atual->pilhas[i]);
        i++;
    }
    novo->total_regras = atual->total_regras;
    novo->regras = atual->regras;
    novo->total_auto_regras = atual->total_auto_regras;
    novo->auto_regras = atual->auto_regras;
    novo->anterior = atual;
    return novo;
}

// Função que limpa o lixo da memória
void libertar_estado(EstadoJogo *e) {
    int i = 0;
    while (i < e->total_pilhas) {
        if (e->pilhas[i].quantidade > 0) free(e->pilhas[i].cartas);
        i++;
    }
    free(e->pilhas);
    free(e);
}

// Função que realiza o UNDO
EstadoJogo* fazer_undo(EstadoJogo *atual) {
    EstadoJogo *anterior = atual;
    if (atual->anterior != NULL) {
        anterior = atual->anterior;
        libertar_estado(atual);
    }
    return anterior;
}
