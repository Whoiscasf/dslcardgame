#include <CUnit/CUnit.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../include/estruturas.h"
#include "../include/logica.h"
#include "../include/reutilizado.h"
#include "../include/dsl.h"

//
// Testes de cartas e vitória
//

// Função que testa se o baralho é criado com sucesso
void test_criar_baralho(void) {
    Carta *deck = criarBaralho(1);
    CU_ASSERT_PTR_NOT_NULL(deck);
    CU_ASSERT_EQUAL(deck[0].num, 1);
    CU_ASSERT_EQUAL(deck[13].naipe, 1);
    free(deck);
}

// Função que testa se quando as cartas são baralhadas não ocorrem erros
void test_baralhar_cartas(void) {
    Carta *deck = criarBaralho(1);
    baralharCartas(deck, 52);
    CU_ASSERT_PTR_NOT_NULL(deck);
    free(deck);
}

// Teste que verifica se a vitória é corretamente detetada
void test_verificar_vitoria(void) {
    EstadoJogo jogo; Pilha p1;
    jogo.total_pilhas = 1; jogo.pilhas = &p1;
    strcpy(p1.nome_tipo, "TAB");
    p1.quantidade = 3;
    CU_ASSERT_EQUAL(verificar_vitoria(&jogo), 0);
    p1.quantidade = 0;
    CU_ASSERT_EQUAL(verificar_vitoria(&jogo), 1);
}

//
// Testes para regras básicas do jogo
//

// Função que testa as regras de sequencia de cor
void test_regra_cor_sequencia(void) {
    Carta c4 = {4, 1};
    Carta c5 = {5, 1};
    Carta c5_preto = {5, 0};
    CU_ASSERT_EQUAL(regra_sequencia(c4, c5, '<'), 1);
    CU_ASSERT_EQUAL(regra_sequencia(c4, c5, '>'), 0);
    CU_ASSERT_EQUAL(regra_cor(c4, c5), 0);
    CU_ASSERT_EQUAL(regra_cor(c4, c5_preto), 1);
}

// Função que testa a implimentação das regras no jogo
void test_aplicar_regras(void) {
    Carta co = {2, 1}; Carta cd = {3, 0};
    CU_ASSERT_EQUAL(aplicar_regras(co, cd, "D<"), 1);
    CU_ASSERT_EQUAL(aplicar_regras(co, cd, "M<"), 0);
}

// Função auxiliar para preparar dados
void aux_prep_regras(EstadoJogo *e, Regra *r, Pilha p[]) {
    strcpy(p[0].nome_tipo, "T1");   p[0].quantidade = 1;
    strcpy(p[1].nome_tipo, "T2");   p[1].quantidade = 1;
    strcpy(p[2].nome_tipo, "L");    p[2].quantidade = 1;
    strcpy(r->origem, "T1"); strcpy(r->destino, "T2"); strcpy(r->flags, "D<");
    e->total_regras = 1; e->regras = r;
}

// Função que testa se a regra é encontrada
void test_encontrar_regra(void) {
    EstadoJogo e; Regra r1; Pilha p[3];
    aux_prep_regras(&e, &r1, p);
    CU_ASSERT_STRING_EQUAL(encontrar_regra(&e, &p[0], &p[1]), "D<");
    CU_ASSERT_PTR_NULL(encontrar_regra(&e, &p[0], &p[2]));
}

// Função que testa se o computador encontra as regras de automatico
void test_tratar_auto(void) {
    EstadoJogo e;
    e.total_auto_regras = 0;
    e.auto_regras = NULL;
    tratar_auto(&e, "TAB", "FUND", "AV");
    CU_ASSERT_EQUAL(e.total_auto_regras, 1);
    CU_ASSERT_STRING_EQUAL(e.auto_regras[0].origem, "TAB");
    CU_ASSERT_STRING_EQUAL(e.auto_regras[0].destino, "FUND");
    CU_ASSERT_STRING_EQUAL(e.auto_regras[0].flags, "AV");
    free(e.auto_regras);
}

// Função que testa se os movimentos autocaticos estão a ser feitos de maneira correta
void test_verificar_vazio(void) {
    Carta as = {1, 0};
    Carta rei = {13, 0};
    Carta normal = {5, 0};
    CU_ASSERT_EQUAL(verificar_vazio(as, "AV"), 1);
    CU_ASSERT_EQUAL(verificar_vazio(rei, "AV"), 0);
    CU_ASSERT_EQUAL(verificar_vazio(rei, "KV"), 1);
    CU_ASSERT_EQUAL(verificar_vazio(normal, "K"), 0);
    CU_ASSERT_EQUAL(verificar_vazio(normal, "V"), 1);
}

// Função que verifica se a regra 'V' bate certo com o estado de espaço ocupado/vazio
void test_regra_combina_estado(void) {
    CU_ASSERT_EQUAL(regra_combina_estado(0, "AV"), 1);
    CU_ASSERT_EQUAL(regra_combina_estado(1, "AV"), 0);
    CU_ASSERT_EQUAL(regra_combina_estado(1, "D<"), 1);
    CU_ASSERT_EQUAL(regra_combina_estado(0, "D<"), 0);
}

// Função que verifica se o sistema reconhece um bloco de cartas perfeito
void test_verificar_bloco(void) {
    Pilha p; Carta c[3];
    c[0].num = 5; c[0].naipe = 1;
    c[1].num = 4; c[1].naipe = 1;
    c[2].num = 3; c[2].naipe = 1;
    p.cartas = c; p.quantidade = 3;
    CU_ASSERT_EQUAL(verificar_bloco(&p, 3), 1);
    c[1].naipe = 2;
    CU_ASSERT_EQUAL(verificar_bloco(&p, 3), 0);
}

// Função que verifica as novas barreiras de segurança para espaços vazios
void test_decidir_vazio(void) {
    Carta as = {1, 0}, rei = {13, 0}, normal = {5, 0};
    Pilha fund, tab;
    strcpy(fund.nome_tipo, "FUND");
    strcpy(tab.nome_tipo, "TAB");
    CU_ASSERT_EQUAL(decidir_vazio(as, &fund, "AV", 1), 1);
    CU_ASSERT_EQUAL(decidir_vazio(normal, &fund, "AV", 1), 0);
    CU_ASSERT_EQUAL(decidir_vazio(rei, &tab, "KV", 1), 1);
    CU_ASSERT_EQUAL(decidir_vazio(normal, &tab, "0", 1), 1);
}

//
// Teste de movimentos
//

// Função auxiliar para criar cartas e pilhas que vão ser usadas nos testes de movimento
void preparar_cartas_e_pilhas(Pilha *p, Carta *co, Carta *cd) {
    memset(p, 0, 2 * sizeof(Pilha));
    co->num = 2; co->naipe = 1;
    cd->num = 3; cd->naipe = 0;
    strcpy(p[0].nome_tipo, "P1");
    p[0].quantidade = 1; p[0].cartas = co;
    strcpy(p[1].nome_tipo, "P2");
    p[1].quantidade = 1; p[1].cartas = cd;
}

// Função auxiliar que cria regras e o estado para ser usado nos testes de movimento
void preparar_regras_e_estado(EstadoJogo *e, Pilha *p, Regra *r) {
    strcpy(r->origem, "P1");
    strcpy(r->destino, "P2");
    strcpy(r->flags, "D<");
    e->total_pilhas = 2; e->pilhas = p;
    e->total_regras = 1; e->regras = r;
}

// Função que testa um movimento entre duas cartas de cor diferente
void test_movimento_normal(void) {
    Pilha p[2]; Carta co, cd;
    preparar_cartas_e_pilhas(p, &co, &cd);
    CU_ASSERT_EQUAL(verificar_movimento(&p[0], &p[1], "D<", 1), 1);
}

// Função que testa o movimento para uma pilha vazia quando as regras não o permitem
void test_mov_vazio_falso(void) {
    Pilha p[2]; Carta co, cd;
    preparar_cartas_e_pilhas(p, &co, &cd);
    p[1].quantidade = 0;
    CU_ASSERT_EQUAL(verificar_movimento(&p[0], &p[1], "K", 1), 0);
}

// Função que testa o movimento para uma pilha vazia quando as regras permitem
void test_mov_vazio_verdadeiro(void) {
    Pilha p[2]; Carta co, cd;
    preparar_cartas_e_pilhas(p, &co, &cd);
    p[1].quantidade = 0;
    CU_ASSERT_EQUAL(verificar_movimento(&p[0], &p[1], "0", 1), 1);
}

// Função que testa se a funbção testar_destinos consegue encontrar um destino válido num cenário possível
void test_testar_destinos(void) {
    EstadoJogo e; Pilha p[2]; Regra r; Carta co, cd;
    preparar_cartas_e_pilhas(p, &co, &cd);
    preparar_regras_e_estado(&e, p, &r);
    int d_o, d_d;
    CU_ASSERT_EQUAL(testar_destinos(&e, 0, &d_o, &d_d), 1);
}

// Função que testa se conseguiu pedir dica
void test_pedir_dica(void) {
    EstadoJogo e; Pilha p[2]; Regra r; Carta co, cd;
    preparar_cartas_e_pilhas(p, &co, &cd);
    preparar_regras_e_estado(&e, p, &r);
    int d_o, d_d;
    CU_ASSERT_EQUAL(pedir_dica(&e, &d_o, &d_d), 1);
}

// Função que testa o movimento de cartas entre duas pilhas
void test_executar_movimento_real(void) {
    Pilha o, d; o.quantidade = 2; d.quantidade = 0;
    o.cartas = malloc(2 * sizeof(Carta)); d.cartas = NULL;
    executar_movimento_real(&o, &d, 1);
    CU_ASSERT_EQUAL(o.quantidade, 1);
    CU_ASSERT_EQUAL(d.quantidade, 1);
    CU_ASSERT_PTR_NOT_NULL(d.cartas);
    free(o.cartas); free(d.cartas);
}

//
// Testes da funcionalidade UNDO
//

// Função que testa se é realizada a cópia da pilha de maneira correta
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

// Função que testa se o UNDO é feito corretamente
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

//
// Motor do CUNIT
//

// Primeira parte dos testes
void adicionarTestes_parte1(CU_pSuite suite) {
    CU_add_test(suite, "test_criar_baralho", test_criar_baralho);
    CU_add_test(suite, "test_baralhar_cartas", test_baralhar_cartas);
    CU_add_test(suite, "test_verificar_vitoria", test_verificar_vitoria);
    CU_add_test(suite, "test_regra_cor_sequencia", test_regra_cor_sequencia);
    CU_add_test(suite, "test_aplicar_regras", test_aplicar_regras);
    CU_add_test(suite, "test_encontrar_regra", test_encontrar_regra);
    CU_add_test(suite, "test_movimento_normal", test_movimento_normal);
    CU_add_test(suite, "test_mov_vazio_falso", test_mov_vazio_falso);
}

// Segunda parte dos testes
void adicionarTestes_parte2(CU_pSuite suite) {
    CU_add_test(suite, "test_mov_vazio_verdadeiro", test_mov_vazio_verdadeiro);
    CU_add_test(suite, "test_testar_destinos", test_testar_destinos);
    CU_add_test(suite, "test_pedir_dica", test_pedir_dica);
    CU_add_test(suite, "test_executar_movimento_real", test_executar_movimento_real);
    CU_add_test(suite, "test_copiar_pilha", test_copiar_pilha);
    CU_add_test(suite, "test_fluxo_undo", test_fluxo_undo);
    CU_add_test(suite, "test_tratar_auto", test_tratar_auto);
    CU_add_test(suite, "test_verificar_vazio", test_verificar_vazio);
}

// Terceira parte dos testes
void adicionarTestes_parte3(CU_pSuite suite) {
    CU_add_test(suite, "test_regra_combina_estado", test_regra_combina_estado);
    CU_add_test(suite, "test_verificar_bloco", test_verificar_bloco);
    CU_add_test(suite, "test_decidir_vazio", test_decidir_vazio);
}

// Função que configura a suite
int configurarSuite(void) {
    CU_pSuite suite = CU_add_suite("Suite_Principal", NULL, NULL);
    if (NULL == suite) {
        CU_cleanup_registry();
        return 0;
    }
    adicionarTestes_parte1(suite);
    adicionarTestes_parte2(suite);
    adicionarTestes_parte3(suite);
    return 1;
}

// Função que verifica se há testes a falhar
int verificarSeFalhou(CU_pTest pTest) {
    int falhou = 0;
    CU_pFailureRecord pFailure = CU_get_failure_list();
    while (pFailure != NULL && falhou == 0) {
        if ((*pFailure).pTest == pTest) falhou = 1;
        pFailure = (*pFailure).pNext;
    }
    return falhou;
}

// Função auxiliar que imprime se o teste passou ou não
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

// Função para imprimir os resultados dos testes
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

//
// Main
//

int main(void) {
    if (CUE_SUCCESS != CU_initialize_registry()) return CU_get_error();
    if (configurarSuite() == 0) return CU_get_error();
    CU_run_all_tests();
    imprimirResultados();
    CU_cleanup_registry();
    return CU_get_error();
}
