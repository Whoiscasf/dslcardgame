#include "../../include/logica.h"
#include <stdlib.h>

// Função que executa o movimento físico das cartas
void executar_movimento_real(Pilha *orig, Pilha *dest, int qtd) {
    int i = 0;
    int nova_qtd_d = dest->quantidade + qtd;
    int nova_qtd_o = orig->quantidade - qtd;
    dest->cartas = realloc(dest->cartas, nova_qtd_d * sizeof(Carta));
    while (i < qtd) {
        dest->cartas[dest->quantidade + i] = orig->cartas[nova_qtd_o + i];
        i++;
    }
    dest->quantidade = nova_qtd_d;
    orig->quantidade = nova_qtd_o;
    if (orig->quantidade > 0)
        orig->cartas = realloc(orig->cartas, orig->quantidade * sizeof(Carta));
    else {
        free(orig->cartas);
        orig->cartas = NULL;
    }
}