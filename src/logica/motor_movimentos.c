#include "../../include/logica.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Função que procura a regra exata para usar ao mover uma carta da pilha
char* encontrar_regra(EstadoJogo *e, char *tipo_orig, char *tipo_dest) {
    int i = 0;
    char *encontrada = NULL;
    while (i < e->total_regras && encontrada == NULL) {
        if (strcmp(e->regras[i].origem, tipo_orig) == 0 &&
            strcmp(e->regras[i].destino, tipo_dest) == 0) {
            encontrada = e->regras[i].flags;
        }
        i++;
    }
    return encontrada;
}

// Função que verifica se as cartas estão em sequência
int regra_sequencia(Carta c1, Carta c2, char sinal) {
    if (sinal == '<') return (c1.num == c2.num - 1);
    if (sinal == '>') return (c1.num == c2.num + 1);
    if (sinal == 'V') return (c1.num == c2.num - 1 || c1.num == c2.num + 1);
    return 1;
}

// Função que verifica se as cores são diferentes
int regra_cor(Carta c1, Carta c2) {
    int cor1 = (c1.naipe == 1 || c1.naipe == 2);
    int cor2 = (c2.naipe == 1 || c2.naipe == 2);
    return (cor1 != cor2);
}

// Função auxiliar que verifica apenas cor e naipe
int aplicar_regras_naipe(Carta c_o, Carta c_d, char *flags) {
    if (strchr(flags, 'D') && !regra_cor(c_o, c_d)) return 0;
    if (strchr(flags, 'M') && c_o.naipe != c_d.naipe) return 0;
    return 1;
}

// Função auxiliar que verifica apenas a direção (< e >)
int aplicar_regras_direcao(Carta c_o, Carta c_d, char *flags) {
    if (strchr(flags, '<') && !regra_sequencia(c_o, c_d, '<')) return 0;
    if (strchr(flags, '>') && !regra_sequencia(c_o, c_d, '>')) return 0;
    return 1;
}

// Função auxiliar que verifica apenas os vizinhos (V e ~)
int aplicar_regras_vizinhos(Carta c_o, Carta c_d, char *flags) {
    if (strchr(flags, 'V') && !regra_sequencia(c_o, c_d, 'V')) return 0;
    if (strchr(flags, '~') && !regra_sequencia(c_o, c_d, 'V')) return 0;
    return 1;
}

// Função que aplica as regras com recurso às auxiliares
int aplicar_regras(Carta c_o, Carta c_d, char *flags) {
    if (!aplicar_regras_naipe(c_o, c_d, flags)) return 0;
    if (!aplicar_regras_direcao(c_o, c_d, flags)) return 0;
    if (!aplicar_regras_vizinhos(c_o, c_d, flags)) return 0;
    return 1;
}

// Função que usa as anteriores para verificar se um movimento é válido
int verificar_movimento(Pilha *o, Pilha *d, char *flags, int q) {
    if (q <= 0 || q > o->quantidade) return 0;
    if (flags == NULL) return 0;
    if (d->quantidade == 0) {
        if (strchr(flags, '0') != NULL) return 1;
        return 0;
    }
    Carta c_origem = o->cartas[o->quantidade - q];
    Carta c_destino = d->cartas[d->quantidade - 1];
    return aplicar_regras(c_origem, c_destino, flags);
}

//
// Sistema de dicas
//

// Função que pega na carta de topo de uma pilha e verifica testando uma a uma qual é a pilha para a qual ela pode mover
int testar_destinos(EstadoJogo *e, int o, int *dica_o, int *dica_d) {
    int d = 0;
    while (d < e->total_pilhas) {
        if (o != d) {
            char *tipo_o = e->pilhas[o].nome_tipo;
            char *tipo_d = e->pilhas[d].nome_tipo;
            char *flags = encontrar_regra(e, tipo_o, tipo_d);
            if (verificar_movimento(&e->pilhas[o], &e->pilhas[d], flags, 1)) {
                *dica_o = o;
                *dica_d = d;
                return 1;
            }
        }
        d++;
    }
    return 0;
}

// Função que percorre as pilhas chamando a função anterior para criar uma dica
int pedir_dica(EstadoJogo *e, int *dica_o, int *dica_d) {
    int o = 0;
    while (o < e->total_pilhas) {
        if (e->pilhas[o].quantidade > 0) {
            if (testar_destinos(e, o, dica_o, dica_d)) return 1;
        }
        o++;
    }
    return 0;
}