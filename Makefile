CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

# 1. Os ficheiros de lógica
BASE = src/dsl/leitor_dsl.c \
       src/dsl/save_load.c \
       src/interface/interface.c \
       src/interface/interface_reutilizada.c \
       src/logica/movimentos.c \
       src/logica/vitoria.c \
       src/logica/motor_movimentos.c \
       src/logica/undo.c \
       src/reutilizado/cartas_reutilizadas.c

# 2. Jogo
all:
	$(CC) $(CFLAGS) src/main.c $(BASE) -o jogo

# 3. Testes
testes:
	$(CC) $(CFLAGS) src/testes/testes.c $(BASE) -o testes -lcunit
	./testes

# 4. PMCCABE
pmccabe:
	pmccabe -v src/main.c src/testes/testes.c $(BASE)

# 5. Clean
clean:
	rm -f jogo testes
