#ifndef INTERFACE_H
#define INTERFACE_H

#include "estruturas.h"

// Funções em interface.c
void imprimirCarta(Carta c, int destaque);
int obterMaxAltura(EstadoJogo *e);
void imprimir_cabecalhos(EstadoJogo *e, int dica_o, int dica_d);
void imprimir_linha_tabuleiro(EstadoJogo *e, int h, int dica_o, int dica_d);
void imprimir_detalhe_pilha(Pilha *p, int num_coluna, int destaque);
void imprimir_outras_pilhas(EstadoJogo *e, int dica_o, int dica_d);
void mostrarTabuleiro(EstadoJogo *e, int dica_o, int dica_d);
int listar_jogos(char ficheiros[][50]);
void imprimir_cabecalho_menu(void);
int menuPrincipal(char *caminho);
void pedirJogada(int *orig, int *dest, int *qtd);

// Função em interface_reutilizada.c
void vitoria(void);

#endif