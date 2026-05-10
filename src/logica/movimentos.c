#include "../../include/logica.h"
#include <stdlib.h>

// Função que executa o movimento físico das cartas
void executar_movimento_real(Pilha *orig, Pilha *dest, int qtd) {
    int i = 0;
    int nova_qtd_d = dest->quantidade + qtd;
    int nova_qtd_o = orig->quantidade - qtd;

    // Aloca espaço no destino
    dest->cartas = realloc(dest->cartas, nova_qtd_d * sizeof(Carta));

    // Copia as cartas
    while (i < qtd) {
        dest->cartas[dest->quantidade + i] = orig->cartas[nova_qtd_o + i];
        i++;
    }

    dest->quantidade = nova_qtd_d;
    orig->quantidade = nova_qtd_o;

    // Ajusta memória da origem
    if (orig->quantidade > 0)
        orig->cartas = realloc(orig->cartas, orig->quantidade * sizeof(Carta));
    else {
        free(orig->cartas);
        orig->cartas = NULL;
    }
}

/*
NOTAS DE CODIGO
    executar_movimento_real:
        Pega num determinado número de cartas da pilha de origem e copia-as para a pilha de destino.
        A parte mais importante aqui é o uso do realloc. Ele serve para esticar ou encolher a memória das pilhas
        à medida que as cartas entram ou saem, garantindo que o programa não gasta mais memória do que a estritamente
        necessária.
 */