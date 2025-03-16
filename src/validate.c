#include <stdlib.h>
#include "validate.h"

// Verifica si un movimiento es válido
int is_valid_move(ChessBoard* board, int from_row, int from_col, int to_row, int to_col) {
    Piece *piece = board->squares[from_row][from_col];

    // PEONES
    if (piece->type == PAWN) {
        int direction = (piece->color == WHITE) ? -1 : 1;  // Blancos van hacia arriba (-1), negros hacia abajo (+1)

        // Movimiento normal de 1 casilla hacia adelante
        if (to_row == from_row + direction && to_col == from_col && board->squares[to_row][to_col] == NULL) {
            return 1;
        }

        // Movimiento doble desde la posición inicial
        if ((piece->color == WHITE && from_row == 6) || (piece->color == BLACK && from_row == 1)) {
            if (to_row == from_row + 2 * direction && to_col == from_col && board->squares[to_row][to_col] == NULL &&
                board->squares[from_row + direction][to_col] == NULL) {
                return 1;
            }
        }

        // Captura en diagonal
        if (to_row == from_row + direction && (to_col == from_col + 1 || to_col == from_col - 1)) {
            if (board->squares[to_row][to_col] != NULL && board->squares[to_row][to_col]->color != piece->color) {
                return 1;
            }
        }

        return 0;  // Cualquier otro movimiento es inválido
    }

    // TODO: Agregar reglas para otras piezas

    return 0;
}