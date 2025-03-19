# Compilador
CC = gcc

# Flags de compilación
CFLAGS = -Wall -Wextra -Werror -I src -I src/board -I src/display -I src/game -I src/input -I src/validate -I src/move

# Nombre del ejecutable
EXEC = chess

# Directorio de código fuente
SRC_DIR = src

# Buscar automáticamente todos los archivos .c dentro de src/
SRC = main.c $(wildcard $(SRC_DIR)/*/*.c)

# Convertir archivos .c en archivos .o
OBJ = $(SRC:.c=.o)

# Regla principal: compilar todo
all: $(EXEC)

# Compilar el ejecutable
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

# Regla para compilar cada .c en su correspondiente .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpiar archivos compilados
clean:
	rm -f $(OBJ) $(EXEC)

# Recompilar desde cero
re: clean all

.PHONY: all clean re
