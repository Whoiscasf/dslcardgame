#include "../../include/dsl.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Função que funciona como um "filtro". Define se uma linha deve ser lida ou ignorada
int linha_valida(char *linha) {
    return (linha[0] != '#' && linha[0] != '\n' && linha[0] != '\0' && strlen(linha) > 1);
}

// Função que prepara uma nova pilha (coluna) de cartas no tabuleiro quando encontrar o comando INIT
void tratar_init(EstadoJogo *e, char *tipo, int n) {
    e->pilhas = realloc(e->pilhas, (e->total_pilhas + 1) * sizeof(Pilha));
    Pilha *p = &e->pilhas[e->total_pilhas];

    strcpy(p->nome_tipo, tipo);
    p->quantidade = n;

    // MUDANÇA: Se a pilha começar com 0 cartas, evitamos o malloc
    if (n > 0) {
        p->cartas = malloc(n * sizeof(Carta));
    } else {
        p->cartas = NULL;
    }

    e->total_pilhas++;
}

// Função que regista as regras de como as cartas se podem mover entre pilhas quando encontra o comando MOV
void tratar_mov(EstadoJogo *e, char *orig, char *dest, char *flags) {
    e->regras = realloc(e->regras, (e->total_regras + 1) * sizeof(Regra));
    strcpy(e->regras[e->total_regras].origem, orig);
    strcpy(e->regras[e->total_regras].destino, dest);
    strcpy(e->regras[e->total_regras].flags, flags);
    e->total_regras++;
}

// Função que percebe qual é o comando específico dentro de uma linha válida
void processar_comando(char *linha, EstadoJogo *e) {
    // MUDANÇA: Aumentámos as variáveis de 20 para 50 espaços
    char p1[50], p2[50], p3[50];
    int n;

    // O TIPO não aloca memória, apenas existe para não dar erro se estiver no texto
    if (sscanf(linha, "TIPO %s %s", p1, p2) == 2) {
        // Não faz nada à memória para já
    }
    // Sempre que houver um INIT, chamamos a tratar_init
    else if (sscanf(linha, "INIT %s %d", p1, &n) == 2) {
        tratar_init(e, p1, n);
    }
    else if (sscanf(linha, "MOV %s %s %s", p1, p2, p3) == 3) {
        tratar_mov(e, p1, p2, p3);
    }
    else if (sscanf(linha, "BARALHOS %d", &n) == 1) {
        e->num_baralhos = n;
    }
    else if (sscanf(linha, "JOGO %s", p1) == 1) {
        printf("Carregando: %s\n", p1);
    }
}

// Função prinipal deste módulo
// Com o nome do ficheiro, usa o mesmo para poder carregar o jogo
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



/*
NOTA DE CODIGO
    linha_valida:
        Se a linha começar com # (um comentário), for uma quebra de linha (\n),
        ou estiver vazia, ela retorna 0 (falso). Só deixa passar linhas com conteúdo útil.

    tratar_init:
        Aumenta o espaço na memória para as pilhas usando realloc. Depois, guarda
        o nome do tipo de pilha, o número de cartas (n), aloca espaço para essas cartas com
        malloc e atualiza o contador total de pilhas.

    tratar_mov:
        Semelhante ao tratar_init, aumenta o espaço na memória para as regras (realloc) e copia a origem,
        o destino e as flags para a nova regra.

    processar_comando:
        Usa o sscanf para "partir" a linha em palavras e números. Dependendo da primeira palavra
        (INIT, MOV, BARALHOS ou JOGO), chama a função correspondente ou guarda o valor diretamente no EstadoJogo

    carregar_paciencia:
        Abre o ficheiro em modo de leitura ("r"). Lê o ficheiro linha a linha com um ciclo while.
        Se a linha passar no filtro do linha_valida, envia-a para o processar_comando. No fim,
        fecha o ficheiro e avisa se teve sucesso.
 */