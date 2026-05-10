#ifndef INTERFACE_H
#define INTERFACE_H

#include "estruturas.h"

// Avisos (assinaturas) de todas as funções que podem ser usadas noutros ficheiros
void imprimirCarta(Carta c, int destaque);
int obterMaxAltura(EstadoJogo *e);
void mostrarTabuleiro(EstadoJogo *e, int dica_o, int dica_d);
int menuPrincipal(char *caminho);
void pedirJogada(int *orig, int *dest, int *qtd);

// AQUI ESTÁ A CORREÇÃO: Avisamos o C que a função vitoria existe!
void vitoria(void);

#endif