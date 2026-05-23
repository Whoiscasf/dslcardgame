#include <stdlib.h>
#include "../../include/estruturas.h"

// Cria um array dinâmico de cartas. O tamanho depende do comando "BARALHOS <n>"
Carta* criarBaralho(int num_baralhos) {
    int total = 52 * num_baralhos;
    Carta *baralho = (Carta*) malloc(total * sizeof(Carta));
    int i = 0;
    while (i < total) {
        baralho[i].num = (i % 13) + 1;
        baralho[i].naipe = (i / 13) % 4;
        i++;
    }
    return baralho;
}

// Baralha as cartas usando o metodo Fisher-Yates
void baralharCartas(Carta *baralho, int total_cartas) {
    int i = total_cartas - 1;
    while (i > 0) {
        int r = rand() % (i + 1);
        Carta temp = baralho[i];
        baralho[i] = baralho[r];
        baralho[r] = temp;
        i--;
    }
}
