#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

// Estrutura de uma carta
typedef struct {
    int num;
    int naipe;
} Carta;

// Estrutura de uma Pilha (Tabuleiro, Stock, etc.)
typedef struct {
    char nome_tipo[20];
    Carta *cartas;
    int quantidade;
    char flags[10];
} Pilha;

// Estrutura para guardar as Regras (comandos MOV)
typedef struct {
    char origem[20];
    char destino[20];
    char flags[20];
} Regra;

// O Estado completo do jogo
typedef struct EstadoJogo {
    Pilha *pilhas;
    int total_pilhas;
    Regra *regras;
    int total_regras;
    int num_baralhos;
    int total_auto_regras;
    Regra *auto_regras;
    struct EstadoJogo *anterior;
} EstadoJogo;

#endif
