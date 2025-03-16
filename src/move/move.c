#include "move.h"
#include "validate.h"
#include <stdio.h>
#include <stdlib.h>

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

//TODO: gestionar las pizas comidas