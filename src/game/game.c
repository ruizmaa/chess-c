#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "input.h"
#include "game.h"
#include "board.h"
#include "validate.h"

GameStatus game_status = { .captured_pieces = {NULL}, .captured_count = 0 };


// Manejar el turno del jugador
void play_turn(ChessBoard* board, const PieceColor current_turn) {
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

// Función para mover una pieza
int move_piece(ChessBoard* board, const int from_row, const int from_col, const int to_row, const int to_col) {
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