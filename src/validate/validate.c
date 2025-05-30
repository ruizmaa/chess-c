#include "validate.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int validation_silent = 0;

// TODO: comprobar cons

// Return 1 cuando la casilla destino tiene pieza aliada
int is_friendly_occupied(const PieceColor color_turn, const Piece *destination) {
    return (destination != NULL && destination->color == color_turn);
}

// Verifica si un movimiento es válido
int is_valid_move(const ChessBoard *board, const int from_row, const int from_col, const int to_row, const int to_col) {
    Piece *piece = board->squares[from_row][from_col];

    if (!piece) {
        show_invalid_reason("No hay ninguna pieza en la casilla de origen");
        return 0;
    }

    if (is_friendly_occupied(piece->color, board->squares[to_row][to_col])) {
        show_invalid_reason("No puedes moverte donde hay una pieza amiga. Intenta de nuevo");
        return 0; // Hay una pieza amiga en el destino
    }
    int valid = 0;
    // TODO: terminar los algoritmos de validacion (captura al paso)
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
        show_invalid_reason("Tipo de pieza desconocido");
        return 0;
    }

    if (!valid) {
        printf("Intételo de nuevo");
        return 0;
    } // No es movimiento valido

    int check;
    check = would_cause_check((ChessBoard *)board, from_row, from_col, to_row, to_col, piece->color);

    if (check) {
        show_invalid_reason("Movimiento que causa un jaque");
        return 0;
    }

    return 1; // Everything ok
}

int is_valid_pawn_move(const ChessBoard *board, const int from_row, const int from_col, const int to_row,
                       const int to_col) {
    PieceColor pawn_color = board->squares[from_row][from_col]->color;

    // Blancos van hacia arriba (-1), negros hacia abajo (+1)
    int direction = (pawn_color == WHITE) ? -1 : 1;

    // Movimiento normal (1 casilla hacia adelante)
    if (to_row == from_row + direction && to_col == from_col) {
        if (board->squares[to_row][to_col] == NULL) {
            return 1;
        } else {
            show_invalid_reason("No puedes avanzar: hay una pieza bloqueando el camino");
            return 0;
        }
    }

    // Movimiento doble desde la posición inicial (2 casillas hacia adelante)
    if ((pawn_color == WHITE && from_row == 6) || (pawn_color == BLACK && from_row == 1)) {
        if (to_row == from_row + 2 * direction && to_col == from_col) {
            if (board->squares[to_row][to_col] != NULL || board->squares[from_row + direction][to_col] != NULL) {
                show_invalid_reason("No puedes avanzar dos casillas: el camino está bloqueado");
                return 0;
            }
            return 1;
        }
    }

    // Capturas
    if (to_row == from_row + direction && (to_col == from_col + 1 || to_col == from_col - 1)) {
        // Captura al paso
        if (board->status.passant_target_row == to_row && board->status.passant_target_col == to_col) {
            int captured_row = (pawn_color == WHITE) ? to_row + 1 : to_row - 1;
            Piece *enemy = board->squares[captured_row][to_col];

            if (enemy != NULL && enemy->type == PAWN && enemy->color != pawn_color) {
                return 1; // Movimiento válido al paso
            } else {
                show_invalid_reason("No hay un peón rival válido para capturar al paso");
                return 0;
            }
        }

        // Caputura en diagonal
        Piece *target = board->squares[to_row][to_col];
        if (target != NULL) {
            if (target->color != pawn_color) {
                return 1;
            } else {
                show_invalid_reason("No puedes capturar tu propia pieza");
                return 0;
            }
        } else {
            show_invalid_reason("No hay ninguna pieza para capturar en esa casilla");
            return 0;
        }
    }

    show_invalid_reason("Movimiento de peón inválido");
    return 0; // Cualquier otro movimiento es inválido
}

int is_valid_rook_move(const ChessBoard *board, const int from_row, const int from_col, const int to_row,
                       const int to_col) {
    // No es un movimiento recto
    if (from_row != to_row && from_col != to_col) {
        show_invalid_reason("La torre solo puede moverse en línea recta horizontal o vertical");
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

        // Hay una pieza en el camino
        if (board->squares[check_row][check_col] != NULL) {
            show_invalid_reason("Hay una pieza en el camino");
            return 0;
        }
    }

    return 1;
}

int is_valid_knight_move(const int from_row, const int from_col, const int to_row, const int to_col) {
    int row_diff = abs(to_row - from_row);
    int col_diff = abs(to_col - from_col);

    if ((row_diff == 2 && col_diff == 1) || (row_diff == 1 && col_diff == 2)) { return 1; }

    show_invalid_reason("El caballo debe moverse en forma de L");
    return 0;
}

int is_valid_bishop_move(const ChessBoard *board, const int from_row, const int from_col, const int to_row,
                         const int to_col) {
    // No es un movimiento diagonal
    if (abs(to_row - from_row) != abs(to_col - from_col)) {
        show_invalid_reason("El alfil solo se puede mover en diagonal");
        return 0;
    }

    // Saber distancia y si va hacia up-right / down-right / down-left / up-left
    int row_step = (to_row > from_row) ? 1 : -1;
    int col_step = (to_col > from_col) ? 1 : -1;
    int distance = abs(to_row - from_row);

    // Verificar piezas intermedias
    for (int i = 1; i < distance; i++) {
        // Hay una pieza bloqueando el camino
        if (board->squares[from_row + i * row_step][from_col + i * col_step] != NULL) {
            show_invalid_reason("Hay una pieza en el camino");
            return 0;
        }
    }

    return 1;
}

int is_valid_queen_move(const ChessBoard *board, const int from_row, const int from_col, const int to_row,
                        const int to_col) {
    if (is_valid_rook_move(board, from_row, from_col, to_row, to_col)) return 1;
    if (is_valid_bishop_move(board, from_row, from_col, to_row, to_col)) return 1;

    show_invalid_reason("La reina no puede moverse así. No es un movimiento válido como torre ni como alfil");
    return 0;
}

int is_valid_king_move(const int from_row, const int from_col, const int to_row, const int to_col) {
    int row_diff = abs(to_row - from_row);
    int col_diff = abs(to_col - from_col);

    if (row_diff <= 1 && col_diff <= 1) { return 1; }

    if (abs(to_col - from_col) == 2 && row_diff == 0) {
        show_invalid_reason(
            "El enroque solo es posible si no hay jaque, ni piezas en el camino, y ni rey ni torre se han movido.");
        return 0;
    }
    show_invalid_reason("El rey solo puede moverse 1 casilla");
    return 0;
}

int is_valid_castling_move(const ChessBoard *board, int from_row, int from_col, int to_row, int to_col) {
    Piece *king = board->squares[from_row][from_col];
    if (!king || king->type != KING) {
        show_invalid_reason("Solo se puede comprobar un movimiento de rey si la ficha seleccionada es un rey");
        return 0;
    }
    // Solo se puede enrocar su fila del rey
    if (from_row != to_row) {
        show_invalid_reason("El rey solo puede enrocar su misma fila");
        return 0;
    }

    // Movimiento debe ser exactamente dos columnas a izquierda o derecha
    int col_diff = to_col - from_col;
    if (abs(col_diff) != 2) {
        show_invalid_reason("El rey no puede enrocarse en esa posición");
        return 0;
    }

    PieceColor color = king->color;
    int rook_col = (col_diff > 0) ? 7 : 0; // Torre en columna 0 (enroque largo) o 7 (enroque corto)
    Piece *rook = board->squares[from_row][rook_col];

    // La torre tiene que ser del mismo color
    if (!rook || rook->type != ROOK || rook->color != color) {
        show_invalid_reason("Solo te puedes enrocar con tu propia torre");
        return 0;
    }

    // Verificar que ni el rey ni la torre se hayan movido
    if (color == WHITE) {
        if (board->status.white_king_moved) {
            show_invalid_reason("El rey ya se ha movido");
            return 0;
        }
        if (rook_col == 7 && board->status.white_rook_kingside_moved) {
            show_invalid_reason("La torre ya se ha movido");
            return 0;
        }
        if (rook_col == 0 && board->status.white_rook_queenside_moved) {
            show_invalid_reason("La torre ya se ha movido");
            return 0;
        }
    } else { // BLACK
        if (board->status.black_king_moved) {
            show_invalid_reason("El rey ya se ha movido");
            return 0;
        }
        if (rook_col == 7 && board->status.black_rook_kingside_moved) {
            show_invalid_reason("La torre ya se ha movido");
            return 0;
        }
        if (rook_col == 0 && board->status.black_rook_queenside_moved) {
            show_invalid_reason("La torre ya se ha movido");
            return 0;
        }
    }

    // Casillas entre rey y torre deben estar vacías
    int step = (col_diff > 0) ? 1 : -1;
    for (int col = from_col + step; col != rook_col; col += step) {
        if (board->squares[from_row][col]) {
            show_invalid_reason("Hay una pieza en el camino");
            return 0;
        }
    }

    PieceColor enemy_color = (color == WHITE) ? BLACK : WHITE;

    // Verificar que el rey no esté en jaque ni pase por casillas atacadas
    // casilla inicial
    if (is_square_attacked(board, from_row, from_col, enemy_color)) {
        show_invalid_reason("El rey está en jaque");
        return 0;
    }
    int middle_col = (from_col + to_col) / 2;
    // casilla intermedia
    if (is_square_attacked(board, from_row, middle_col, enemy_color)) {
        show_invalid_reason("Una casilla intermedia está siendo atacada");
        return 0;
    }
    // casilla final
    if (is_square_attacked(board, to_row, to_col, enemy_color)) {
        show_invalid_reason("La casilla final está siendo atacada");
        return 0;
    }
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
    ChessBoard temp_board;
    memcpy(&temp_board, board, sizeof(ChessBoard));

    Piece *moving_piece = temp_board.squares[from_row][from_col];

    // Simular el movimiento
    temp_board.squares[to_row][to_col] = moving_piece;
    temp_board.squares[from_row][from_col] = NULL;

    // Usar is_in_check() directamente sobre el tablero simulado
    return is_in_check(&temp_board, color);
}

int is_in_check(const ChessBoard *board, PieceColor color) {
    SILENT_BLOCK_START
    int king_row = -1, king_col = -1;

    // Buscar la posición del rey
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            Piece *p = board->squares[row][col];
            if (p && p->type == KING && p->color == color) {
                king_row = row;
                king_col = col;
                break;
            }
        }
        if (king_row != -1) break;
    }

    if (king_row == -1 || king_col == -1) {
        fprintf(stderr, "Error: rey del color %s no encontrado en el tablero.\n", color == WHITE ? "blanco" : "negro");
        return 1;
    }

    PieceColor enemy = (color == WHITE) ? BLACK : WHITE;
    int result = is_square_attacked(board, king_row, king_col, enemy);
    SILENT_BLOCK_END
    return result;
}

void show_invalid_reason(const char *reason) {
    if (!validation_silent) { printf("Movimiento inválido: %s\n", reason); }
}