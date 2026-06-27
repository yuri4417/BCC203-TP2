CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -MMD -MP 
LDLIBS = -lm  

SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=build/%.o)
DEP = $(OBJ:.o=.d)

TARGET = ordena
VERIF_SRC = test/verifica.c
VERIF_OBJ = build/verifica.o
VERIF_TARGET = build/verifica

all: $(TARGET) $(VERIF_TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDLIBS)

$(VERIF_TARGET): $(VERIF_OBJ)
	$(CC) $(VERIF_OBJ) -o $(VERIF_TARGET) $(LDLIBS)

build/%.o: src/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

build/verifica.o: test/verifica.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(TARGET)


# Testes/Benchmarks
M ?= 1
Q ?= 1000
S ?= 3

test: all
	@./test/test-run.sh $(M) $(Q) $(S)

# Benchmark completo (todas situações, tamanhos escalonados, 5 repetições)
bench: all
	@./test/bench.sh $(M)

clean:
	rm -rf build $(TARGET) logs/*

valgrind: all
	valgrind --leak-check=full ./$(TARGET)

-include $(DEP)