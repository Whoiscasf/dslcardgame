#include <CUnit/CUnit.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../include/estruturas.h"
#include "../include/logica.h"
#include "../include/reutilizado.h"

/* =========================================================
 * 1. TESTES: CARTAS E VITÓRIA
 * ========================================================= */

void test_criar_baralho(void) {
    Carta *deck = criarBaralho(1);
    CU_ASSERT_PTR_NOT_NULL(deck);
    CU_ASSERT_EQUAL(deck[0].num, 1);
    CU_ASSERT_EQUAL(deck[13].naipe, 1);
    free(deck);
}

void test_baralhar_cartas(void) {
    Carta *deck = criarBaralho(1);
    baralharCartas(deck, 52);
    CU_ASSERT_PTR_NOT_NULL(deck);
    free(deck);
}

void test_verificar_vitoria(void) {
    EstadoJogo jogo; Pilha p1;
    jogo.total_pilhas = 1; jogo.pilhas = &p1;
    strcpy(p1.nome_tipo, "TAB");

    p1.quantidade = 3;
    CU_ASSERT_EQUAL(verificar_vitoria(&jogo), 0);

    p1.quantidade = 0;
    CU_ASSERT_EQUAL(verificar_vitoria(&jogo), 1);
}

/* =========================================================
 * 2. TESTES: REGRAS SIMPLES
 * ========================================================= */

void test_regra_cor_sequencia(void) {
    Carta c4 = {4, 1};
    Carta c5 = {5, 1};
    Carta c5_preto = {5, 0};

    CU_ASSERT_EQUAL(regra_sequencia(c4, c5, '<'), 1);
    CU_ASSERT_EQUAL(regra_sequencia(c4, c5, '>'), 0);
    CU_ASSERT_EQUAL(regra_cor(c4, c5), 0);
    CU_ASSERT_EQUAL(regra_cor(c4, c5_preto), 1);
}

void test_aplicar_regras(void) {
    Carta co = {2, 1}; Carta cd = {3, 0};
    CU_ASSERT_EQUAL(aplicar_regras(co, cd, "D<"), 1);
    CU_ASSERT_EQUAL(aplicar_regras(co, cd, "M<"), 0);
}

void test_encontrar_regra(void) {
    EstadoJogo e; Regra r1;
    strcpy(r1.origem, "T1"); strcpy(r1.destino, "T2"); strcpy(r1.flags, "D<");
    e.total_regras = 1; e.regras = &r1;

    CU_ASSERT_STRING_EQUAL(encontrar_regra(&e, "T1", "T2"), "D<");
    CU_ASSERT_PTR_NULL(encontrar_regra(&e, "T1", "LIXO"));
}

/* =========================================================
 * 3. AUXILIARES PARA SETUP DE MOVIMENTOS (< 15 instruções)
 * ========================================================= */

void preparar_cartas_e_pilhas(Pilha *p, Carta *co, Carta *cd) {
    memset(p, 0, 2 * sizeof(Pilha)); // Limpa o "lixo" da memória!

    co->num = 2; co->naipe = 1;
    cd->num = 3; cd->naipe = 0;

    strcpy(p[0].nome_tipo, "P1");
    p[0].quantidade = 1; p[0].cartas = co;

    strcpy(p[1].nome_tipo, "P2");
    p[1].quantidade = 1; p[1].cartas = cd;
}

void preparar_regras_e_estado(EstadoJogo *e, Pilha *p, Regra *r) {
    strcpy(r->origem, "P1");
    strcpy(r->destino, "P2");
    strcpy(r->flags, "D<");
    e->total_pilhas = 2; e->pilhas = p;
    e->total_regras = 1; e->regras = r;
}

/* =========================================================
 * 4. TESTES: MOVIMENTOS E DICAS
 * ========================================================= */

void test_movimento_normal(void) {
    Pilha p[2]; Carta co, cd;
    preparar_cartas_e_pilhas(p, &co, &cd);
    // Testa apenas o movimento de 2 vermelho para 3 preto
    CU_ASSERT_EQUAL(verificar_movimento(&p[0], &p[1], "D<", 1), 1);
}

void test_mov_vazio_falso(void) {
    Pilha p[2]; Carta co, cd;
    preparar_cartas_e_pilhas(p, &co, &cd);
    p[1].quantidade = 0; // Esvazia o destino
    // Tenta mover com regra que não permite vazios
    CU_ASSERT_EQUAL(verificar_movimento(&p[0], &p[1], "D<", 1), 0);
}

void test_mov_vazio_verdadeiro(void) {
    Pilha p[2]; Carta co, cd;
    preparar_cartas_e_pilhas(p, &co, &cd);
    p[1].quantidade = 0; // Esvazia o destino
    // Tenta mover com a flag '0' que permite vazios
    CU_ASSERT_EQUAL(verificar_movimento(&p[0], &p[1], "0", 1), 1);
}

void test_testar_destinos(void) {
    EstadoJogo e; Pilha p[2]; Regra r; Carta co, cd;
    preparar_cartas_e_pilhas(p, &co, &cd);
    preparar_regras_e_estado(&e, p, &r);

    int d_o, d_d;
    CU_ASSERT_EQUAL(testar_destinos(&e, 0, &d_o, &d_d), 1);
}

void test_pedir_dica(void) {
    EstadoJogo e; Pilha p[2]; Regra r; Carta co, cd;
    preparar_cartas_e_pilhas(p, &co, &cd);
    preparar_regras_e_estado(&e, p, &r);

    int d_o, d_d;
    CU_ASSERT_EQUAL(pedir_dica(&e, &d_o, &d_d), 1);
}

void test_executar_movimento_real(void) {
    Pilha o, d; o.quantidade = 2; d.quantidade = 0;
    o.cartas = malloc(2 * sizeof(Carta)); d.cartas = NULL;

    executar_movimento_real(&o, &d, 1);

    CU_ASSERT_EQUAL(o.quantidade, 1);
    CU_ASSERT_EQUAL(d.quantidade, 1);
    CU_ASSERT_PTR_NOT_NULL(d.cartas);

    free(o.cartas); free(d.cartas);
}

/* =========================================================
 * 5. TESTES: UNDO (Dividido para pmccabe)
 * ========================================================= */

void test_copiar_pilha(void) {
    Pilha o, d;
    strcpy(o.nome_tipo, "TAB"); o.quantidade = 1;
    o.cartas = malloc(sizeof(Carta));

    copiar_pilha(&d, &o);

    CU_ASSERT_STRING_EQUAL(d.nome_tipo, "TAB");
    CU_ASSERT_EQUAL(d.quantidade, 1);
    CU_ASSERT_PTR_NOT_NULL(d.cartas);

    free(o.cartas); free(d.cartas);
}

void test_fluxo_undo(void) {
    EstadoJogo *e1 = malloc(sizeof(EstadoJogo));
    e1->total_pilhas = 0; e1->total_regras = 0; e1->anterior = NULL;

    EstadoJogo *e2 = guardar_estado(e1);
    CU_ASSERT_PTR_NOT_NULL(e2);
    CU_ASSERT_PTR_EQUAL(e2->anterior, e1);

    EstadoJogo *e_revertido = fazer_undo(e2);
    CU_ASSERT_PTR_EQUAL(e_revertido, e1);

    free(e1);
}

/* =========================================================
 * 6. MOTOR DE IMPRESSÃO PERSONALIZADO
 * ========================================================= */

void adicionarTestes(CU_pSuite suite) {
    CU_add_test(suite, "test_criar_baralho", test_criar_baralho);
    CU_add_test(suite, "test_baralhar_cartas", test_baralhar_cartas);
    CU_add_test(suite, "test_verificar_vitoria", test_verificar_vitoria);
    CU_add_test(suite, "test_regra_cor_sequencia", test_regra_cor_sequencia);
    CU_add_test(suite, "test_aplicar_regras", test_aplicar_regras);
    CU_add_test(suite, "test_encontrar_regra", test_encontrar_regra);
    CU_add_test(suite, "test_movimento_normal", test_movimento_normal);
    CU_add_test(suite, "test_mov_vazio_falso", test_mov_vazio_falso);
    CU_add_test(suite, "test_mov_vazio_verdadeiro", test_mov_vazio_verdadeiro);
    CU_add_test(suite, "test_testar_destinos", test_testar_destinos);
    CU_add_test(suite, "test_pedir_dica", test_pedir_dica);
    CU_add_test(suite, "test_executar_movimento_real", test_executar_movimento_real);
    CU_add_test(suite, "test_copiar_pilha", test_copiar_pilha);
    CU_add_test(suite, "test_fluxo_undo", test_fluxo_undo);
}

int configurarSuite(void) {
    CU_pSuite suite = CU_add_suite("Suite_Principal", NULL, NULL);
    if (NULL == suite) {
        CU_cleanup_registry();
        return 0;
    }
    adicionarTestes(suite);
    return 1;
}

int verificarSeFalhou(CU_pTest pTest) {
    int falhou = 0;
    CU_pFailureRecord pFailure = CU_get_failure_list();
    while (pFailure != NULL && falhou == 0) {
        if ((*pFailure).pTest == pTest) falhou = 1;
        pFailure = (*pFailure).pNext;
    }
    return falhou;
}

void imprimirTestesDaSuite(CU_pSuite pSuite) {
    CU_pTest pTest = (*pSuite).pTest;
    while (pTest != NULL) {
        if (verificarSeFalhou(pTest) == 1)
            printf("Funcao %-35s : \033[31mNAO PASSOU\033[0m\n", (*pTest).pName);
        else
            printf("Funcao %-35s : \033[32mPASSOU\033[0m\n", (*pTest).pName);
        pTest = (*pTest).pNext;
    }
}

void imprimirResultados(void) {
    printf("\n=======================================================\n");
    printf("                  RESULTADO DOS TESTES                 \n");
    printf("=======================================================\n");
    CU_pSuite pSuite = (*CU_get_registry()).pSuite;
    while (pSuite != NULL) {
        imprimirTestesDaSuite(pSuite);
        pSuite = (*pSuite).pNext;
    }
    printf("=======================================================\n\n");
}

/* =========================================================
 * MAIN
 * ========================================================= */

int main(void) {
    if (CUE_SUCCESS != CU_initialize_registry()) return CU_get_error();
    if (configurarSuite() == 0) return CU_get_error();

    // Corre tudo de forma invisível
    CU_run_all_tests();

    // Lê a memória interna do CUnit e imprime
    imprimirResultados();

    CU_cleanup_registry();
    return CU_get_error();
}