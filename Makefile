# Compilador
CC = gcc

# Flags comunes
INCLUDES = -I src -I src/board -I src/display -I src/game -I src/input -I src/validate -I src/move

# Flags de compilación
CFLAGS = -Wall -Wextra -Werror $(INCLUDES)

# Debug
DEBUG_FLAGS = -g -Wall -Wextra -Werror $(INCLUDES)

# Nombre del ejecutable
EXEC = chess
DEBUG_EXEC = chess_debug

# Directorio de código fuente
SRC_DIR = src

# Buscar automáticamente todos los archivos .c dentro de src/
SRC = main.c $(wildcard $(SRC_DIR)/*/*.c)

# Convertir archivos .c en archivos .o
OBJ = $(SRC:.c=.o)
DEBUG_OBJ = $(SRC:.c=.debug.o)

# Regla principal: compilar todo
all: $(EXEC)

# Compilar el ejecutable
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

# Compilar el ejecutable de debug
debug: $(DEBUG_EXEC)

$(DEBUG_EXEC): $(DEBUG_OBJ)
	$(CC) $(DEBUG_OBJ) -o $(DEBUG_EXEC)

# Regla para compilar cada .c en su correspondiente .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.debug.o: %.c
	$(CC) $(DEBUG_FLAGS) -c $< -o $@

# Limpiar archivos compilados
clean:
	rm -f $(OBJ) $(DEBUG_OBJ) $(EXEC) $(DEBUG_EXEC)

# Recompilar desde cero
re: clean all

# Formatear todos los archivos .c y .h en src/
format:
	find src -name "*.c" -o -name "*.h" | xargs clang-format -i

.PHONY: all clean re format
