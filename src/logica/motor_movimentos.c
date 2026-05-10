#include "../../include/logica.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Procura a regra exata para usar ao mover uma carta da pilha
char* encontrar_regra(EstadoJogo *e, char *tipo_orig, char *tipo_dest) {
    int i = 0;
    char *encontrada = NULL;

    while (i < e->total_regras && encontrada == NULL) {
        if (strcmp(e->regras[i].origem, tipo_orig) == 0 &&
            strcmp(e->regras[i].destino, tipo_dest) == 0) {
            encontrada = e->regras[i].flags;
        }
        i++;
    }
    return encontrada;
}

// Verifica se as cartas estão em sequência
int regra_sequencia(Carta c1, Carta c2, char sinal) {
    if (sinal == '<') return (c1.num == c2.num - 1);
    if (sinal == '>') return (c1.num == c2.num + 1);
    if (sinal == 'V') return (c1.num == c2.num - 1 || c1.num == c2.num + 1);
    return 1;
}

// Verifica se as cores são diferentes
int regra_cor(Carta c1, Carta c2) {
    int cor1 = (c1.naipe == 1 || c1.naipe == 2);
    int cor2 = (c2.naipe == 1 || c2.naipe == 2);
    return (cor1 != cor2);
}

// Função auxiliar que verifica apenas cor e naipe
int aplicar_regras_naipe(Carta c_o, Carta c_d, char *flags) {
    if (strchr(flags, 'D') && !regra_cor(c_o, c_d)) return 0;
    if (strchr(flags, 'M') && c_o.naipe != c_d.naipe) return 0;
    return 1;
}

// Função auxiliar que verifica apenas a direção (< e >)
int aplicar_regras_direcao(Carta c_o, Carta c_d, char *flags) {
    if (strchr(flags, '<') && !regra_sequencia(c_o, c_d, '<')) return 0;
    if (strchr(flags, '>') && !regra_sequencia(c_o, c_d, '>')) return 0;
    return 1;
}

// Função auxiliar que verifica apenas os vizinhos (V e ~)
int aplicar_regras_vizinhos(Carta c_o, Carta c_d, char *flags) {
    if (strchr(flags, 'V') && !regra_sequencia(c_o, c_d, 'V')) return 0;
    if (strchr(flags, '~') && !regra_sequencia(c_o, c_d, 'V')) return 0;
    return 1;
}

// A Função Principal (agora super leve!)
int aplicar_regras(Carta c_o, Carta c_d, char *flags) {
    if (!aplicar_regras_naipe(c_o, c_d, flags)) return 0;
    if (!aplicar_regras_direcao(c_o, c_d, flags)) return 0;
    if (!aplicar_regras_vizinhos(c_o, c_d, flags)) return 0;

    return 1;
}

// Função que usa as anteriores para verificar se um movimento é válido
int verificar_movimento(Pilha *o, Pilha *d, char *flags, int q) {
    // 1. Validação de segurança básica: não pode mover zero cartas nem mais do que tem
    if (q <= 0 || q > o->quantidade) return 0;

    // 2. A BARREIRA DO VAZIO (Onde estava o teu bug!)
    if (d->quantidade == 0) {
        // Se as flags tiverem o caracter '0', permite o movimento
        if (strchr(flags, '0') != NULL) return 1;

        // Se não tiver o '0', chumba imediatamente
        return 0;
    }

    // 3. Se a pilha de destino não está vazia, agarramos nas cartas a comparar
    Carta c_origem = o->cartas[o->quantidade - q];
    Carta c_destino = d->cartas[d->quantidade - 1];

    // 4. Envia para o motor de validação
    return aplicar_regras(c_origem, c_destino, flags);
}

//
// Sistema de dicas
//

// Função que pega na carta de topo de uma pilha e verifica testando uma a uma qual é a pilha para a qual ela pode mover
int testar_destinos(EstadoJogo *e, int o, int *dica_o, int *dica_d) {
    int d = 0;
    while (d < e->total_pilhas) {
        if (o != d) {
            char *tipo_o = e->pilhas[o].nome_tipo;
            char *tipo_d = e->pilhas[d].nome_tipo;
            char *flags = encontrar_regra(e, tipo_o, tipo_d);

            if (verificar_movimento(&e->pilhas[o], &e->pilhas[d], flags, 1)) {
                *dica_o = o;
                *dica_d = d;
                return 1;
            }
        }
        d++;
    }
    return 0;
}

// Função que percorre as pilhas chamando a função anterior para criar uma dica
int pedir_dica(EstadoJogo *e, int *dica_o, int *dica_d) {
    int o = 0;
    while (o < e->total_pilhas) {
        if (e->pilhas[o].quantidade > 0) {
            // Atira para a função auxiliar testar todos os destinos
            if (testar_destinos(e, o, dica_o, dica_d)) return 1;
        }
        o++;
    }
    return 0;
}

/*
NOTAS DE CODIGO
    encontrar_regra:
        Imagina isto como um dicionário. Quando queres mover uma carta de uma pilha (ex: "Mão") para outra
        (ex: "Tabuleiro"), esta função vai procurar na lista de regras do jogo qual é a regra específica (as "flags")
        que se aplica a essa troca. Devolve as letras/símbolos que ditam as regras.

    regra_sequencia:
        Faz a matemática básica. Ela recebe duas cartas e um símbolo de direção:
        •	<: A carta de origem tem de ter um número abaixo da destino (ex: 4 para o 5).
        •	>: A carta de origem tem de ter um número acima da destino (ex: 6 para o 5).
        •	V: Pode ser tanto para cima como para baixo.

    regra_cor:
        Verifica se duas cartas são de cores diferentes. Ela assume que os naipes 1 e 2 são de uma cor (ex: vermelho) e
        os restantes de outra (preto). Devolve 1 (verdadeiro) se as cores forem diferentes.

    aplicar_regras:
        A função aplicar_regras verifica se um movimento entre duas cartas é válido segundo as regras presentes na string
        flags. Para cada regra encontrada, a função testa a condição correspondente, como sequência crescente, decrescente,
        cores diferentes ou mesmo naipe. Caso alguma regra falhe, a função devolve 0, indicando que o movimento é inválido.
        Se todas as regras forem cumpridas, devolve 1, permitindo o movimento.

    verificar_movimento:
        •	strchr(flags, '0'): Esta é uma função fantástica da biblioteca <string.h> do C.
            Ela procura a letra '0' dentro do texto das flags (ex: "D<0"). Se encontrar, devolve a posição; se não encontrar,
            devolve NULL (vazio).
        •	A ordem importa: Ao colocarmos o if (d->quantidade == 0) logo no topo, protegemos o programa. Ele toma a decisão
            de rejeitar o movimento antes de tentar ler a variável c_destino, evitando ler lixo na memória.

    testar_destinos:
        É uma função auxiliar usada apenas para te dar dicas. Ela pega numa pilha específica que tu tens e vai tentar
        jogar a carta do topo dessa pilha em todas as outras pilhas do tabuleiro. Se o verificar_movimento disser que sim,
        ela guarda esses números para te avisar "Olha, podes mover daqui para ali!".

    pedir_dica:
        O mestre do testar_destinos. Ele vai olhar para a primeira pilha do jogo e mandar testar; se não der nada,
        olha para a segunda; depois para a terceira. Mal encontre um movimento possível, ele pára e devolve essa dica para
        o ecrã.
 */