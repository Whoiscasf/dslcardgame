#include "../../include/logica.h"
#include <stdlib.h>
#include <string.h>

// Função que cria uma cópia de uma pilha
void copiar_pilha(Pilha *dest, Pilha *orig) {
    int i = 0;

    // Copiar o nome da pilha para a vitória não falhar!
    strcpy(dest->nome_tipo, orig->nome_tipo);

    dest->quantidade = orig->quantidade;

    // Só faz malloc se houver cartas
    if (orig->quantidade > 0) {
        dest->cartas = malloc(orig->quantidade * sizeof(Carta));
        while (i < orig->quantidade) {
            dest->cartas[i] = orig->cartas[i];
            i++;
        }
    } else {
        dest->cartas = NULL;
    }
}

// Função que guarda o estado atual
EstadoJogo* guardar_estado(EstadoJogo *atual) {
    EstadoJogo *novo = malloc(sizeof(EstadoJogo));
    int i = 0;

    novo->total_pilhas = atual->total_pilhas;
    novo->pilhas = malloc(atual->total_pilhas * sizeof(Pilha));

    while (i < atual->total_pilhas) {
        copiar_pilha(&novo->pilhas[i], &atual->pilhas[i]);
        i++;
    }

    novo->total_regras = atual->total_regras;
    novo->regras = atual->regras; // Mantém as mesmas regras

    novo->anterior = atual; // Encadeia
    return novo;
}

// Função nova para limpar o lixo da memória
void libertar_estado(EstadoJogo *e) {
    int i = 0;
    while (i < e->total_pilhas) {
        if (e->pilhas[i].quantidade > 0) free(e->pilhas[i].cartas);
        i++;
    }
    free(e->pilhas);
    free(e);
}

// Função que realiza o UNDO
EstadoJogo* fazer_undo(EstadoJogo *atual) {
    EstadoJogo *anterior = atual;
    if (atual->anterior != NULL) {
        anterior = atual->anterior;
        libertar_estado(atual); // Apaga a "fotografia" da jogada que revertemos!
    }
    return anterior;
}

/*
NOTAS DE CODIGO
    copiar_pilha:
        A copiar_pilha trata de fazer o clone de uma única pilha:
            1.  Copia o Nome: Copia o "nome_tipo" da pilha. O teu próprio código avisa: "para a vitória não falhar!".
                Isto é porque a função da vitória precisa de saber quais são as pilhas do "Tabuleiro" para ver se estão vazias.

            2.	Reserva Memória (malloc): Ela não diz apenas "esta carta é aquela". Ela pede espaço de memória novo e
            independente ao computador (malloc) apenas para a quantidade certa de cartas.

            3.	Copia as Cartas: Uma a uma, enfia as cartas na nova memória reservada.
        Se a pilha estivesse vazia, ela simplesmente diz que as cartas são NULL (não há nada para copiar), poupando
        memória ao computador.

    guardar_estado:
        Antes de fazeres um movimento novo, esta função tira uma "fotografia" ao estado do jogo atual.
        Ela copia todas as pilhas e cartas para um sítio seguro.

    libertar_estado:
        Esta função vai libertar a memória do estado atual quando for fazer UNDO

    fazer_undo:
        Se carregares na tecla para voltar atrás, ele chama o libertar_estado para deitar ao lixo as cartas do momento
        presente (limpando a memória para não o jogo não bloquear) e carrega a "fotografia" do turno anterior.
 */