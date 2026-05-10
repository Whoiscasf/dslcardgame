#include "../../include/dsl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
// --- PARTE 1: GUARDAR O JOGO ---
//

// Função que traduz o número do naipe qu eestá na memória para a letra correspondente para escrever o ficheiro
char obter_naipe_char(int n) {
    const char naipes[] = "SHDC"; // 0:Espadas(S), 1:Copas(H), 2:Ouros(D), 3:Paus(C)
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
    fprintf(f, "\n"); // Mesma a pilha vazia precisa de uma nova linha
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
// --- PARTE 2: CARREGAR O JOGO ---
//

// Função que converte o naipe em números
int converter_naipe(char n) {
    if (n == 'H') return 1;
    if (n == 'D') return 2;
    if (n == 'C') return 3;
    return 0; // 'S'
}

// Função que converte as cartas especiais em números
int converter_valor(char *v) {
    if (v[0] == 'A') return 1;
    if (v[0] == 'J') return 11;
    if (v[0] == 'Q') return 12;
    if (v[0] == 'K') return 13;
    return atoi(v); // Converte "10", "5", etc. em número
}

// Pega numa única linha de texto e transforma-a numa coluna de cartas
void processar_linha_save(Pilha *p, char *linha) {
    free(p->cartas); // Deita fora as cartas geradas pelo INIT
    p->cartas = NULL;
    p->quantidade = 0;

    char *token = strtok(linha, " \n");
    while (token != NULL) {
        // Aloca espaço exato e regista a carta
        p->cartas = realloc(p->cartas, (p->quantidade + 1) * sizeof(Carta));
        int len = strlen(token);
        p->cartas[p->quantidade].naipe = converter_naipe(token[len-1]);
        p->cartas[p->quantidade].num = converter_valor(token);

        p->quantidade++;
        token = strtok(NULL, " \n");
    }
}

// Função que restaura o jogo inteiro
int carregar_save(EstadoJogo *e, const char *ficheiro, char *caminho_lido) {
    FILE *f = fopen(ficheiro, "r");
    if (!f) return 0;
    char linha[512];

    // 1. Lê a primeira linha e guarda qual foi o jogo (Ex: paciencias/golf.txt)
    if (fgets(linha, sizeof(linha), f) && sscanf(linha, "JOGO %s", caminho_lido) == 1) {
        carregar_paciencia(caminho_lido, e); // Cria as colunas certas!

        // 2. Lê as cartas linha a linha e envia para a função auxiliar
        int p = 0;
        while (fgets(linha, sizeof(linha), f) && p < e->total_pilhas) {
            processar_linha_save(&e->pilhas[p], linha);
            p++;
        }
    }
    fclose(f);
    return 1;
}

/*
NOTA DE CODIGO
    obter_naipe_char:
        Recebe 0, 1, 2, 3 e devolve 'S', 'H', 'D', 'C'.

    gravar_pilha:
        Percorre as cartas da pilha uma a uma. Transforma o número da carta na sua representação
        em texto (ex: 1 vira "A", 13 vira "K") usando um array de strings vals. Junta o valor
        ao naipe e escreve no ficheiro.

    guardar_jogo:
        Abre o ficheiro em modo de escrita ("w"). Escreve logo na primeira linha qual é o jogo que
        estamos a jogar (ex: JOGO paciencias/golf.txt). Depois, usa um ciclo para chamar a gravar_pilha
        para cada pilha existente no tabuleiro.

    converter_naipe:
        Recebe H, D, C, S e retorna 1, 2, 3, 0

    converter_valor:
        Recebe as cartas e converte o valor. As cartas especiais A, J, Q, K são logo convertidas nos 'if'.
        Se o char não corresponder ao de uma carta especial, usamos atoi(v) para converter a string em número

    processar_linha_save:
        Primeiro, limpa a memória antiga dessa pilha com free. Depois, usa o strtok para partir a
        linha pelos espaços. Para cada "palavra" (carta) encontrada, aloca espaço, converte o valor e o naipe, e guarda-a.

    carregar_save:
        Abre o ficheiro de save. Lê a primeira linha para descobrir o nome do jogo original e chama o
        carregar_paciencia para criar o tabuleiro vazio com as regras certas. A seguir, lê as restantes
        linhas, passando cada uma para o processar_linha_save, que vai substituir as cartas padrão pelas cartas do
        momento em que o jogo foi guardado.
*/