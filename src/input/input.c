#include <stdio.h>
#include <string.h>

#include "input.h"

// Convierte input usuario  en coordenadas: ("a2") -> (0, 1)
void get_coordinates(const char *input, int *row, int *col) {
    *row = 8 - (input[1] - '0');  // Convierte '2' en 6, '4' en 4, etc.
    *col = input[0] - 'a';  // Convierte 'e' en 4, 'd' en 3, etc.
}

// Pide al usuario unas coordenadas y comprueba con is_valid_input()
void get_user_input(char *input) {
    while (1) {
        scanf("%2s", input);

        if (!is_valid_input(input)) {
            printf("Entrada inválida. Intenta de nuevo: ");
            continue;
        }
        break;
    }
}

// Asegura que el input sea bueno (eg: "a2" OK | "kasf" NO | "c9" NO)
int is_valid_input(const char *input) {
    // Verificar que la longitud sea exactamente 2
    if (strlen(input) != 2) return 0;

    // Verificar que el primer carácter esté entre 'a' y 'h'
    if (input[0] < 'a' || input[0] > 'h') return 0;

    // Verificar que el segundo carácter esté entre '1' y '8'
    if (input[1] < '1' || input[1] > '8') return 0;

    return 1;
}