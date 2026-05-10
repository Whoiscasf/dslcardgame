#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

// Estrutura de uma carta
typedef struct {
    int num;        // 1 a 13
    int naipe;      // 0 a 3
} Carta;

// Estrutura de uma Pilha (Tabuleiro, Stock, etc.)
typedef struct {
    char nome_tipo[20];
    Carta *cartas;      // Array dinâmico
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
    Pilha *pilhas;      // Array com todas as pilhas
    int total_pilhas;
    
    Regra *regras;      // Array com as regras lidas da DSL
    int total_regras;
    
    int num_baralhos;   // Número de baralhos lido da DSL
    
    struct EstadoJogo *anterior; // Apontador para o Undo
} EstadoJogo;

#endif
