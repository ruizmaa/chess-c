#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "game.h"
#include "board.h"
#include "validate.h"

// Manejar el turno del jugador
void play_turn(ChessBoard* board, PieceColor current_turn) {
    // ['a', '2', '/0']
    char from[3], to[3];
    int from_row, from_col, to_row, to_col;
    Piece *selected_piece;

    // Elegir el color del mensaje según el turno
    char *turn_color = (current_turn == WHITE) ? WHITE_COLOR : BLACK_COLOR;

    while (1) {
        // Printeo inicio del turno
        printf("%sTurno de %s.", turn_color, current_turn == WHITE ? "blancas" : "negras");
        printf("%s Selecciona una pieza para mover (ej: e2): ", RESET);

        get_user_input(from);                                                   // Pido un input valido
        get_coordinates(from, &from_row, &from_col);                            // Convertir la entrada en coordenadas
        selected_piece = board->squares[from_row][from_col];                    
        if (!is_valid_from_piece(selected_piece, current_turn, from)) continue; // Validar que la coordenada sea válida (haya una pieza del jugador)

        // Printeo destino
        printf("Selecciona la casilla de destino (ej: e4): ");
        get_user_input(to);                                                     // Pido un input valido
        get_coordinates(to, &to_row, &to_col);                                  // Convertir la entrada en coordenadas
        if (move_piece(board, from_row, from_col, to_row, to_col)) {            // Muevo la ficha
            break;
        } else {
            printf("Movimiento inválido. Intenta de nuevo.\n");
        }
    }
}

// Función para mover una pieza (antes en move.c)
int move_piece(ChessBoard* board, int from_row, int from_col, int to_row, int to_col) {
    Piece *piece = board->squares[from_row][from_col];
    if (!piece) return 0;

    if (!is_valid_move(board, from_row, from_col, to_row, to_col)) return 0;

    if (board->squares[to_row][to_col] != NULL) {
        free(board->squares[to_row][to_col]);
    }

    board->squares[to_row][to_col] = board->squares[from_row][from_col];
    board->squares[from_row][from_col] = NULL;

    return 1;
}

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

// Comprueba que en la coordenada haya una ficha del jugador
int is_valid_from_piece(const Piece *selected_piece, const PieceColor current_turn, const char *from){
    if (!selected_piece) {
        printf("No hay ninguna pieza en %s. Intenta de nuevo.\n", from);
        return 0;
    }
    if (selected_piece->color != current_turn) {
        printf("Esa pieza no es tuya. Intenta de nuevo.\n");
        return 0;
    }
    return 1;
}