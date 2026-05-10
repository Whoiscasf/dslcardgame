#ifndef LOGICA_H
#define LOGICA_H

#include "estruturas.h"

// Motor de Movimentos e Validações
int verificar_movimento(Pilha *orig, Pilha *dest, char *flags, int qtd);
int verificar_vitoria(EstadoJogo *e);
void executar_movimento_real(Pilha *orig, Pilha *dest, int qtd);
int aplicar_regras_naipe(Carta c_o, Carta c_d, char *flags);
char* encontrar_regra(EstadoJogo *e, char *tipo_orig, char *tipo_dest);
// Sistema de Undo
EstadoJogo* guardar_estado(EstadoJogo *atual);
EstadoJogo* fazer_undo(EstadoJogo *atual);

int pedir_dica(EstadoJogo *e, int *dica_o, int *dica_d);

// --- Funções Auxiliares (Expostas para os Testes Unitários) ---
int regra_sequencia(Carta c1, Carta c2, char sinal);
int regra_cor(Carta c1, Carta c2);
int aplicar_regras(Carta c_o, Carta c_d, char *flags);
int testar_destinos(EstadoJogo *e, int o, int *dica_o, int *dica_d);
void copiar_pilha(Pilha *dest, Pilha *orig);

#endif