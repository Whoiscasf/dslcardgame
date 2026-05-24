#include "../../include/dsl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
// Funções para guardar o jogo
//

// Função que traduz o número do naipe que está na memória para a letra correspondente para escrever o ficheiro
char obter_naipe_char(int n) {
    const char naipes[] = "SHDC";
    return naipes[n];
}

// Função que escreve todas as cartas de uma única pilha numa linha de texto
void gravar_pilha(FILE *f, Pilha p) {
    int i = 0;
    const char *vals[] = {"", "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    while (i < p.quantidade) {
        fprintf(f, "%s%c", vals[p.cartas[i].num], obter_naipe_char(p.cartas[i].naipe));
        if (i < p.quantidade - 1) fprintf(f, " ");
        i++;
    }
    fprintf(f, "\n");
}

// Cria o ficheiro 'save' final
int guardar_jogo(EstadoJogo *e, const char *nome_paciencia, const char *ficheiro_save) {
    FILE *f = fopen(ficheiro_save, "w");
    int i = 0;
    if (f == NULL) return 0;
    fprintf(f, "JOGO %s\n", nome_paciencia);
    while (i < e->total_pilhas) {
        gravar_pilha(f, e->pilhas[i]);
        i++;
    }
    fclose(f);
    return 1;
}

//
// Funções para carregar o jogo
//

// Função que converte o naipe em números
int converter_naipe(char n) {
    if (n == 'H') return 1;
    if (n == 'D') return 2;
    if (n == 'C') return 3;
    return 0;
}

// Função que converte as cartas especiais em números
int converter_valor(char *v) {
    if (v[0] == 'A') return 1;
    if (v[0] == 'J') return 11;
    if (v[0] == 'Q') return 12;
    if (v[0] == 'K') return 13;
    return atoi(v);
}

// Pega numa única linha de texto e transforma-a numa coluna de cartas
void processar_linha_save(Pilha *p, char *linha) {
    free(p->cartas);
    p->cartas = NULL;
    p->quantidade = 0;
    char *token = strtok(linha, " \n");
    while (token != NULL) {
        p->cartas = realloc(p->cartas, (p->quantidade + 1) * sizeof(Carta));
        int len = strlen(token);
        p->cartas[p->quantidade].naipe = converter_naipe(token[len-1]);
        p->cartas[p->quantidade].num = converter_valor(token);
        p->quantidade++;
        token = strtok(NULL, " \n");
    }
}

// Função que restaura o jogo inteiro que está no save.txt
int carregar_save(EstadoJogo *e, const char *ficheiro, char *caminho_lido) {
    FILE *f = fopen(ficheiro, "r");
    if (!f) return 0;
    char linha[512];
    if (fgets(linha, sizeof(linha), f) && sscanf(linha, "JOGO %s", caminho_lido) == 1) {
        carregar_paciencia(caminho_lido, e);
        int p = 0;
        while (fgets(linha, sizeof(linha), f) && p < e->total_pilhas) {
            processar_linha_save(&e->pilhas[p], linha);
            p++;
        }
    }
    fclose(f);
    return 1;
}