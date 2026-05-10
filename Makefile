CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

# 1. Os ficheiros de lógica pura (usados em ambos os lados)
BASE = src/dsl/leitor_dsl.c \
       src/dsl/save_load.c \
       src/interface/interface.c \
       src/interface/interface_reutilizada.c \
       src/logica/movimentos.c \
       src/logica/vitoria.c \
       src/logica/motor_movimentos.c \
       src/logica/undo.c \
       src/reutilizado/cartas_reutilizadas.c

# 2. O JOGO PRINCIPAL (Junta a Base com o main.c)
all:
	$(CC) $(CFLAGS) src/main.c $(BASE) -o jogo

# 3. OS TESTES (Juntam a Base com o testes.c e a biblioteca CUnit)
testes:
	$(CC) $(CFLAGS) src/testes/testes.c $(BASE) -o correr_testes -lcunit
	./correr_testes

# 4. AVALIAÇÃO DE COMPLEXIDADE (Avalia o jogo normal e os testes)
pmccabe:
	pmccabe src/main.c src/testes/testes.c $(BASE)

clean:
	rm -f jogo correr_testes