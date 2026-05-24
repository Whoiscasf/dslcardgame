#ifndef PARSER_H
#define PARSER_H
#include <stdio.h>
#include "estruturas.h"

// Funções em leitor_dsl.c
int linha_valida(char *linha);
void tratar_init(EstadoJogo *e, char *tipo, int n);
void tratar_mov(EstadoJogo *e, char *orig, char *dest, char *flags);
void tratar_auto(EstadoJogo *e, char *orig, char *dest, char *flags);
void processar_comando(char *linha, EstadoJogo *e);
int carregar_paciencia(const char *nome_ficheiro, EstadoJogo *e);

// Funções em save_load.c
char obter_naipe_char(int n);
void gravar_pilha(FILE *f, Pilha p);
int guardar_jogo(EstadoJogo *e, const char *nome_paciencia, const char *ficheiro_save);
int converter_naipe(char n);
int converter_valor(char *v);
void processar_linha_save(Pilha *p, char *linha);
int carregar_save(EstadoJogo *e, const char *ficheiro, char *caminho_lido);
#endif
