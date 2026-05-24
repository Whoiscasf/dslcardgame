#include "../../include/dsl.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Função que define se uma linha deve ser lida ou ignorada
int linha_valida(char *linha) {
    return (linha[0] != '#' && linha[0] != '\n' && linha[0] != '\0' && strlen(linha) > 1);
}

// Função que prepara uma nova pilha de cartas no tabuleiro quando encontrar o comando INIT
void tratar_init(EstadoJogo *e, char *tipo, int n) {
    e->pilhas = realloc(e->pilhas, (e->total_pilhas + 1) * sizeof(Pilha));
    Pilha *p = &e->pilhas[e->total_pilhas];
    strcpy(p->nome_tipo, tipo);
    p->quantidade = n;
    if (n > 0) {
        p->cartas = malloc(n * sizeof(Carta));
    } else {
        p->cartas = NULL;
    }
    e->total_pilhas++;
}

// Função que regista as regras de como as cartas se podem mover entre pilhas quando encontra o comando MOV
void tratar_mov(EstadoJogo *e, char *orig, char *dest, char *flags) {
    int i = 0;
    while (flags[i] != '\0') {
        if (flags[i] >= 'a' && flags[i] <= 'z') flags[i] -= 32;
        i++;
    }
    e->regras = realloc(e->regras, (e->total_regras + 1) * sizeof(Regra));
    strcpy(e->regras[e->total_regras].origem, orig);
    strcpy(e->regras[e->total_regras].destino, dest);
    strcpy(e->regras[e->total_regras].flags, flags);
    e->total_regras++;
}

// Função auxiliar para guardar as regras automáticas
void tratar_auto(EstadoJogo *e, char *orig, char *dest, char *flags) {
    int i = 0;
    while (flags[i] != '\0') {
        if (flags[i] >= 'a' && flags[i] <= 'z') flags[i] -= 32;
        i++;
    }
    e->auto_regras = realloc(e->auto_regras, (e->total_auto_regras + 1) * sizeof(Regra));
    strcpy(e->auto_regras[e->total_auto_regras].origem, orig);
    strcpy(e->auto_regras[e->total_auto_regras].destino, dest);
    strcpy(e->auto_regras[e->total_auto_regras].flags, flags);
    e->total_auto_regras++;
}

// Função que percebe qual é o comando específico dentro de uma linha válida
void processar_comando(char *linha, EstadoJogo *e) {
    char p1[50], p2[50], p3[50];
    int n;
    if (sscanf(linha, "TIPO %s %s", p1, p2) == 2) {
    }
    else if (sscanf(linha, "INIT %s %d", p1, &n) == 2) {
        tratar_init(e, p1, n);
    }
    else if (sscanf(linha, "MOV %s %s %s", p1, p2, p3) == 3) {
        tratar_mov(e, p1, p2, p3);
    }
    else if (sscanf(linha, "AUTO %s %s %s", p1, p2, p3) == 3) {
        tratar_auto(e, p1, p2, p3);
    }
    else if (sscanf(linha, "BARALHOS %d", &n) == 1) {
        e->num_baralhos = n;
    }
    else if (sscanf(linha, "JOGO %s", p1) == 1) {
        printf("Carregando: %s\n", p1);
    }
}

// Função que carrega o jogo escolhido
int carregar_paciencia(const char *nome_ficheiro, EstadoJogo *e) {
    FILE *f = fopen(nome_ficheiro, "r");
    char linha[256];
    int sucesso = 0;

    if (f != NULL) {
        while (fgets(linha, sizeof(linha), f)) {
            if (linha_valida(linha)) processar_comando(linha, e);
        }
        fclose(f);
        sucesso = 1;
    }
    return sucesso;
}
