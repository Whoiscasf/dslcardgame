#include <stdlib.h>
#include "../../include/estruturas.h"

// Cria um array dinâmico de cartas. O tamanho depende do comando "BARALHOS <n>"
Carta* criarBaralho(int num_baralhos) {
    int total = 52 * num_baralhos;
    Carta *baralho = (Carta*) malloc(total * sizeof(Carta));
    int i = 0;
    
    while (i < total) {
        baralho[i].num = (i % 13) + 1;    // Dá valores de 1 a 13 ciclicamente
        baralho[i].naipe = (i / 13) % 4;  // Muda de naipe a cada 13 cartas
        i++;
    }
    
    return baralho;
}

// Baralha as cartas usando o teu metodo Fisher-Yates
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

/*
NOTAS DE CODIGO
    criarBaralho:
        Esta função serve para construir as tuas cartas do zero, criando o número exato de baralhos que o jogo pedir.
        •	Calcular o Tamanho: Começa por multiplicar 52 (o número de cartas de um baralho normal) pela quantidade de
            baralhos que precisas (num_baralhos).
        •	Reserva de Memória (malloc): Depois de saber o total, pede ao computador para reservar o espaço de memória
            exato para acomodar todas essas cartas.
        •	A Matemática Mágica (O Ciclo): Em vez de escrever as cartas uma a uma, usa duas fórmulas matemáticas muito
            inteligentes para preencher os dados de todas as cartas rapidamente:
        •	O Número ((i % 13) + 1): O operador % (resto da divisão) obriga a que os valores nunca passem de 12. Ao somar 1,
            garante que os valores das cartas andam sempre num ciclo perfeito de 1 a 13 (Ás ao Rei), voltando ao início de
            cada vez que chega ao 13.
        •	O Naipe ((i / 13) % 4): Ao dividir por 13, o valor só aumenta um número inteiro a cada 13 cartas. O % 4 no fim
            garante que o resultado seja sempre 0, 1, 2 ou 3. Ou seja, cria 13 cartas de um naipe, depois muda de naipe para
            as próximas 13, e assim sucessivamente.

    baralharCartas:
        Depois de criares as cartas, elas estão perfeitamente ordenadas. Esta função usa um algoritmo muito famoso na
        programação (o metodo de Fisher-Yates) para as baralhar de forma rápida e justa.
        •	De Trás para a Frente: O ciclo começa na última carta do baralho (total_cartas - 1) e vai descendo até à primeira.
        •	O Sorteio (r = rand() % (i + 1)): Em cada passo do ciclo, o computador escolhe uma posição aleatória entre a
            carta zero (a primeira) e a carta onde o ciclo está naquele momento.
        •	A Troca (Swap): Pega na carta da posição aleatória (r) e troca-a de lugar com a carta atual (i). A variável
            temp serve apenas como um "bolso" temporário para segurar uma das cartas enquanto fazes a troca. Ao fazer isto uma
            única vez para cada posição, o baralho fica completamente misturado.
 */