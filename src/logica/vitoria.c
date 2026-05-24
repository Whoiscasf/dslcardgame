#include "../../include/logica.h"

// Verifica se o jogo foi ganho (todas as pilhas do tabuleiro estão vazias)
int verificar_vitoria(EstadoJogo *e) {
    int i = 0;
    int ganhou = 1;
    while (i < e->total_pilhas) {
        if (e->pilhas[i].nome_tipo[0] == 'T' && e->pilhas[i].quantidade > 0) {
            ganhou = 0;
        }
        i++;
    }
    return ganhou;
}

