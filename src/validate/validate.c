#include <stdlib.h>
#include "validate.h"

// Verifica si un movimiento es válido
int is_valid_move(const ChessBoard* board, const int from_row, const int from_col, const int to_row, const int to_col) {
    Piece *piece = board->squares[from_row][from_col];

    if (!piece) return 1;

    switch (piece->type) {
        case PAWN:
            return is_valid_pawn_move(board, from_row, from_col, to_row, to_col);
        case ROOK:
            return is_valid_rook_move(board, from_row, from_col, to_row, to_col);
        // case KNIGHT:
        //     return is_valid_knight_move(from_row, from_col, to_row, to_col);
        // case BISHOP:
        //     return is_valid_bishop_move(board, from_row, from_col, to_row, to_col);
        // case QUEEN:
        //     return is_valid_queen_move(board, from_row, from_col, to_row, to_col);
        // case KING:
        //     return is_valid_king_move(board, from_row, from_col, to_row, to_col);
        default:
            return 1;
    }

    return 0;
}

int is_valid_pawn_move(const ChessBoard* board, const int from_row, const int from_col, const int to_row, const int to_col) {
    Piece* piece = board->squares[from_row][from_col];
    
    // Blancos van hacia arriba (-1), negros hacia abajo (+1)
    int direction = (piece->color == WHITE) ? -1 : 1;

    // Movimiento normal (1 casilla hacia adelante)
    if (to_row == from_row + direction && to_col == from_col && board->squares[to_row][to_col] == NULL) {
        return 1;
    }

    // Movimiento doble desde la posición inicial (2 casillas hacia adelante)
    if ((piece->color == WHITE && from_row == 6) || (piece->color == BLACK && from_row == 1)) {
        if (to_row == from_row + 2 * direction && to_col == from_col && board->squares[to_row][to_col] == NULL && board->squares[from_row + direction][to_col] == NULL) {
            return 1;
        }
    }

    // Captura en diagonal
    if (to_row == from_row + direction && (to_col == from_col + 1 || to_col == from_col - 1)) {
        if (board->squares[to_row][to_col] != NULL && board->squares[to_row][to_col]->color != piece->color) {
            return 1;
        }
    }

    //TODO: captura a paso

    return 0;  // Cualquier otro movimiento es inválido
}

int is_valid_rook_move(const ChessBoard* board, int from_row, int from_col, int to_row, int to_col) {
    
    // No es un movimiento recto
    if (from_row != to_row && from_col != to_col) {
        return 0;
    }

    // Direccion movimiento arriba/abajo / derecha/izquierda
    int step = (from_row == to_row) ? (to_col > from_col ? 1 : -1) : (to_row > from_row ? 1 : -1);

    // Total de casillas que se desplaza
    int distance = (from_row == to_row) ? abs(to_col - from_col) : abs(to_row - from_row);
    
    // Verificar piezas intermedias
    for (int i = 1; i < distance; i++) {
        int check_row = (from_row == to_row) ? from_row : from_row + i * step;
        int check_col = (from_col == to_col) ? from_col : from_col + i * step;

        if (board->squares[check_row][check_col] != NULL) {
            return 0;  // Hay una pieza en el camino
        }
    }

    // Verificar la pieza en la casilla destino
    if (board->squares[to_row][to_col] != NULL && board->squares[to_row][to_col]->color == board->squares[from_row][from_col]->color) {
        return 0;  // La pieza en la casilla destino es del mismo color
    }

    return 1;  // El movimiento es válido
}
