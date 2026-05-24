#ifndef LOGICA_H
#define LOGICA_H
#include "estruturas.h"

// Funções em motor_movimentos.c
int regra_combina_estado(int qtd_dest, char *flags);
int verificar_bloco(Pilha *o, int q);
int decidir_vazio(Carta c, Pilha *d, char *flags, int q);
char* encontrar_regra(EstadoJogo *e, Pilha *orig, Pilha *dest);
int regra_sequencia(Carta c1, Carta c2, char sinal);
int regra_cor(Carta c1, Carta c2);
int aplicar_regras_naipe(Carta c_o, Carta c_d, char *flags);
int aplicar_regras_direcao(Carta c_o, Carta c_d, char *flags);
int aplicar_regras_vizinhos(Carta c_o, Carta c_d, char *flags);
int aplicar_regras(Carta c_o, Carta c_d, char *flags);
int verificar_vazio(Carta c_origem, char *flags);
int verificar_movimento(Pilha *orig, Pilha *dest, char *flags, int qtd);
int testar_destinos(EstadoJogo *e, int o, int *dica_o, int *dica_d);
int pedir_dica(EstadoJogo *e, int *dica_o, int *dica_d);

// Funções em movimentos.c
void executar_movimento_real(Pilha *orig, Pilha *dest, int qtd);

// Funções em undo.c
void copiar_pilha(Pilha *dest, Pilha *orig);
EstadoJogo* guardar_estado(EstadoJogo *atual);
void libertar_estado(EstadoJogo *e);
EstadoJogo* fazer_undo(EstadoJogo *atual);

// Funções em vitoria.c
int verificar_vitoria(EstadoJogo *e);

#endif
