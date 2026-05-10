#ifndef PARSER_H
#define PARSER_H

#include "estruturas.h"

int carregar_paciencia(const char *nome_ficheiro, EstadoJogo *e);
int guardar_jogo(EstadoJogo *e, const char *nome_paciencia, const char *ficheiro_save);
int carregar_save(EstadoJogo *e, const char *ficheiro, char *caminho_lido);

#endif