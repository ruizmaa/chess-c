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
        if (!valid) { valid = is_valid_castling_move(board, from_row, from_col, to_row, to_col); }
        break;
    default:
        return 0;
    }

    if (!valid) return 0; // No es movimiento valido

    if (would_cause_check((ChessBoard *)board, from_row, from_col, to_row, to_col, piece->color)) { return 0; }

    return 1; // Everything ok
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

int is_valid_castling_move(const ChessBoard *board, int from_row, int from_col, int to_row, int to_col) {
    Piece *king = board->squares[from_row][from_col];
    if (!king || king->type != KING) return 0;

    // Solo se puede enrocar en la fila del rey
    if (from_row != to_row) return 0;

    // Movimiento debe ser exactamente dos columnas a izquierda o derecha
    int col_diff = to_col - from_col;
    if (abs(col_diff) != 2) return 0;

    PieceColor color = king->color;
    int rook_col = (col_diff > 0) ? 7 : 0; // Torre en columna 0 (enroque largo) o 7 (enroque corto)
    Piece *rook = board->squares[from_row][rook_col];

    // La torre tiene que ser del mismo color
    if (!rook || rook->type != ROOK || rook->color != color) return 0;

    // Verificar que ni el rey ni la torre se hayan movido
    if (color == WHITE) {
        if (board->status.white_king_moved) return 0;
        if (rook_col == 7 && board->status.white_rook_kingside_moved) return 0;
        if (rook_col == 0 && board->status.white_rook_queenside_moved) return 0;
    } else { // BLACK
        if (board->status.black_king_moved) return 0;
        if (rook_col == 7 && board->status.black_rook_kingside_moved) return 0;
        if (rook_col == 0 && board->status.black_rook_queenside_moved) return 0;
    }

    // Casillas entre rey y torre deben estar vacías
    int step = (col_diff > 0) ? 1 : -1;
    for (int col = from_col + step; col != rook_col; col += step) {
        if (board->squares[from_row][col]) return 0;
    }

    PieceColor enemy_color = (color == WHITE) ? BLACK : WHITE;

    // Verificar que el rey no esté en jaque ni pase por casillas atacadas
    if (is_square_attacked(board, from_row, from_col, enemy_color)) return 0; // casilla inicial
    int middle_col = (from_col + to_col) / 2;
    if (is_square_attacked(board, from_row, middle_col, enemy_color)) return 0; // casilla intermedia
    if (is_square_attacked(board, to_row, to_col, enemy_color)) return 0;       // casilla final

    return 1;
}

int is_square_attacked(const ChessBoard *board, const int row, const int col, const PieceColor by_color) {
    for (int row_check = 0; row_check < BOARD_SIZE; row_check++) {
        for (int col_check = 0; col_check < BOARD_SIZE; col_check++) {
            Piece *attacker = board->squares[row_check][col_check];
            if (!attacker || attacker->color != by_color) continue;

            switch (attacker->type) {
            case PAWN:
                // Los peones atacan en diagonal (NO se usa is_valid_pawn_move)
                if (by_color == WHITE) {
                    if ((row_check - 1 == row) && (col_check - 1 == col || col_check + 1 == col)) return 1;
                } else {
                    if ((row_check + 1 == row) && (col_check - 1 == col || col_check + 1 == col)) return 1;
                }
                break;

            case KNIGHT:
                if (is_valid_knight_move(row_check, col_check, row, col)) return 1;
                break;

            case BISHOP:
                if (is_valid_bishop_move(board, row_check, col_check, row, col)) return 1;
                break;

            case ROOK:
                if (is_valid_rook_move(board, row_check, col_check, row, col)) return 1;
                break;

            case QUEEN:
                if (is_valid_queen_move(board, row_check, col_check, row, col)) return 1;
                break;

            case KING:
                if (is_valid_king_move(row_check, col_check, row, col)) return 1;
                break;
            }
        }
    }

    return 0; // Ninguna pieza enemiga amenaza esa casilla
}

int would_cause_check(ChessBoard *board, const int from_row, const int from_col, const int to_row, int to_col,
                      const PieceColor color) {
    Piece *moving_piece = board->squares[from_row][from_col];
    Piece *captured_piece = board->squares[to_row][to_col];

    // Simular el movimiento
    board->squares[to_row][to_col] = moving_piece;
    board->squares[from_row][from_col] = NULL;

    // Buscar la posición actual del rey después del movimiento
    int king_row = -1, king_col = -1;
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            Piece *p = board->squares[r][c];
            if (p && p->type == KING && p->color == color) {
                king_row = r;
                king_col = c;
                break;
            }
        }
        if (king_row != -1) break;
    }

    // Restaurar el estado original
    board->squares[from_row][from_col] = moving_piece;
    board->squares[to_row][to_col] = captured_piece;

    // No se encontró el rey, error grave
    // TODO: quitar esto if (king_row == -1 || king_col == -1) return 1;

    // Verificar si la casilla del rey está atacada
    PieceColor enemy_color = (color == WHITE) ? BLACK : WHITE;
    return is_square_attacked(board, king_row, king_col, enemy_color);
}