#include "validate.h"
#include <stdlib.h>

// TOD: comprobar cons

// Return 1 cuando la casilla destino tiene pieza aliada
int is_friendly_occupied(const PieceColor color_turn, const Piece *destination) {
    return (destination != NULL && destination->color == color_turn);
}

// Verifica si un movimiento es válido
int is_valid_move(const ChessBoard *board, const int from_row, const int from_col, const int to_row, const int to_col) {
    Piece *piece = board->squares[from_row][from_col];

    if (!piece) return 0;

    if (is_friendly_occupied(piece->color, board->squares[to_row][to_col]))
        return 0; // Hay una pieza amiga en el destino

    int valid = 0;
    // TODO: terminar los algoritmos de validacion
    switch (piece->type) {
    case PAWN:
        valid = is_valid_pawn_move(board, from_row, from_col, to_row, to_col);
        break;
    case ROOK:
        valid = is_valid_rook_move(board, from_row, from_col, to_row, to_col);
        break;
    case KNIGHT:
        valid = is_valid_knight_move(from_row, from_col, to_row, to_col);
        break;
    case BISHOP:
        valid = is_valid_bishop_move(board, from_row, from_col, to_row, to_col);
        break;
    case QUEEN:
        valid = is_valid_queen_move(board, from_row, from_col, to_row, to_col);
        break;
    case KING:
        valid = is_valid_king_move(from_row, from_col, to_row, to_col);
        // Enroque
        // if (!valid) {
        //     valid = is_valid_castling_move(board, from_row, from_col, to_row,
        //     to_col);
        // }
        break;
    default:
        return 0;
    }

    if (!valid) return 0; // No es movimiento valido

    // if (is_king_check()) return 0;

    return 1; // Todo ok
}

int is_valid_pawn_move(const ChessBoard *board, const int from_row, const int from_col, const int to_row,
                       const int to_col) {
    PieceColor pawn_color = board->squares[from_row][from_col]->color;

    // Blancos van hacia arriba (-1), negros hacia abajo (+1)
    int direction = (pawn_color == WHITE) ? -1 : 1;

    // Movimiento normal (1 casilla hacia adelante)
    if (to_row == from_row + direction && to_col == from_col && board->squares[to_row][to_col] == NULL) { return 1; }

    // Movimiento doble desde la posición inicial (2 casillas hacia adelante)
    if ((pawn_color == WHITE && from_row == 6) || (pawn_color == BLACK && from_row == 1)) {
        if (to_row == from_row + 2 * direction && to_col == from_col && board->squares[to_row][to_col] == NULL &&
            board->squares[from_row + direction][to_col] == NULL) {
            return 1;
        }
    }

    // Captura en diagonal
    if (to_row == from_row + direction && (to_col == from_col + 1 || to_col == from_col - 1)) {
        if (board->squares[to_row][to_col] != NULL && board->squares[to_row][to_col]->color != pawn_color) { return 1; }
    }

    // TODO: captura a paso

    return 0; // Cualquier otro movimiento es inválido
}

int is_valid_rook_move(const ChessBoard *board, const int from_row, const int from_col, const int to_row,
                       const int to_col) {
    if (from_row != to_row && from_col != to_col) return 0; // No es un movimiento recto

    // Direccion movimiento arriba/abajo / derecha/izquierda
    int step = (from_row == to_row) ? (to_col > from_col ? 1 : -1) : (to_row > from_row ? 1 : -1);
    // Total de casillas que se desplaza
    int distance = (from_row == to_row) ? abs(to_col - from_col) : abs(to_row - from_row);

    // Verificar piezas intermedias
    for (int i = 1; i < distance; i++) {
        int check_row = (from_row == to_row) ? from_row : from_row + i * step;
        int check_col = (from_col == to_col) ? from_col : from_col + i * step;

        if (board->squares[check_row][check_col] != NULL) return 0; // Hay una pieza en el camino
    }

    return 1;
}

int is_valid_knight_move(const int from_row, const int from_col, const int to_row, const int to_col) {
    int row_diff = abs(to_row - from_row);
    int col_diff = abs(to_col - from_col);

    return (row_diff == 2 && col_diff == 1) || (row_diff == 1 && col_diff == 2);
}

int is_valid_bishop_move(const ChessBoard *board, const int from_row, const int from_col, const int to_row,
                         const int to_col) {
    if (abs(to_row - from_row) != abs(to_col - from_col)) return 0; // No es un movimiento diagonal

    // Saber distancia y si va hacia up-right / down-right / down-left / up-left
    int row_step = (to_row > from_row) ? 1 : -1;
    int col_step = (to_col > from_col) ? 1 : -1;
    int distance = abs(to_row - from_row);

    // Verificar piezas intermedias
    for (int i = 1; i < distance; i++) {
        if (board->squares[from_row + i * row_step][from_col + i * col_step] != NULL)
            return 0; // Hay una pieza bloqueando el camino
    }

    return 1;
}

int is_valid_queen_move(const ChessBoard *board, const int from_row, const int from_col, const int to_row,
                        const int to_col) {
    return is_valid_rook_move(board, from_row, from_col, to_row, to_col) ||
           is_valid_bishop_move(board, from_row, from_col, to_row, to_col);
}

int is_valid_king_move(const int from_row, const int from_col, const int to_row, const int to_col) {
    int row_diff = abs(to_row - from_row);
    int col_diff = abs(to_col - from_col);

    return (row_diff <= 1 && col_diff <= 1);
}

// int is_valid_castling_move() {}

// int is_king_in_check() {}