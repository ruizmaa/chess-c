#include "move.h"
#include "validate.h"
#include <stdio.h>
#include <stdlib.h>

// Función para mover una pieza
int move_piece(ChessBoard *board, const int from_row, const int from_col, const int to_row, const int to_col) {
    Piece *piece = board->squares[from_row][from_col];

    // No hay pieza en la posicion de origen
    if (!piece) return 0;

    // Comprueba que el movimiento sea valido
    if (!is_valid_move(board, from_row, from_col, to_row, to_col)) return 0;

    // Captura
    if (!take_piece(board, piece, to_row, to_col)) {
        return 0; // Error al capturar
    }

    // Muevo la pieza a la nueva casilla y vacio la anterior
    board->squares[to_row][to_col] = piece;
    board->squares[from_row][from_col] = NULL;

    // Enroque: mover la torre correspondiente
    if (piece->type == KING && abs(to_col - from_col) == 2) {
        int rook_from_col = (to_col > from_col) ? 7 : 0; // Torre en flanco rey o reina
        int rook_to_col = (to_col > from_col) ? to_col - 1 : to_col + 1;

        Piece *rook = board->squares[to_row][rook_from_col];
        board->squares[to_row][rook_from_col] = NULL;
        board->squares[to_row][rook_to_col] = rook;
    }

    // Actualiza flags de enroque
    if (piece->type == KING) {
        if (piece->color == WHITE)
            board->status.white_king_moved = 1;
        else
            board->status.black_king_moved = 1;
    }
    if (piece->type == ROOK) {
        if (piece->color == WHITE) {
            if (from_row == 7 && from_col == 0)
                board->status.white_rook_queenside_moved = 1;
            else if (from_row == 7 && from_col == 7)
                board->status.white_rook_kingside_moved = 1;
        } else {
            if (from_row == 0 && from_col == 0)
                board->status.black_rook_queenside_moved = 1;
            else if (from_row == 0 && from_col == 7)
                board->status.black_rook_kingside_moved = 1;
        }
    }

    // Gestionar si se habilita comer al paso
    reset_peasant(board, piece, from_row, from_col, to_row);

    return 1;
}

// Comprueba que en la coordenada haya una ficha del jugador
int is_valid_from_piece(const Piece *selected_piece, const PieceColor current_turn, const char *from) {
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

// Gestiona el comer fichas
int take_piece(ChessBoard *board, Piece *piece, const int to_row, const int to_col) {
    // Debug para ver el objetivo de captura al paso
    if (board->status.passant_target_row >= 0 && board->status.passant_target_col >= 0) {
        printf("[DEBUG] Revisando captura al paso: destino %c%d - objetivo passant %c%d\n", 'a' + to_col, 8 - to_row,
               'a' + board->status.passant_target_col, 8 - board->status.passant_target_row);
    } else {
        printf("[DEBUG] Revisando captura al paso: destino %c%d - sin objetivo passant válido\n", 'a' + to_col,
               8 - to_row);
    }

    // Captura al paso
    if (take_piece_peasant(board, piece, to_row, to_col)) { return 1; }

    // Captura normal
    Piece *captured_piece = board->squares[to_row][to_col];
    if (captured_piece) {
        if (board->status.captured_count < MAX_CAPTURES) {
            board->status.captured_pieces[board->status.captured_count++] = captured_piece; // Guardar pieza capturada
        } else {
            printf("Error: Se excedió el límite de piezas capturadas.\n");
            return 0;
        }
    }
    return 1;
}

int take_piece_peasant(ChessBoard *board, Piece *piece, const int to_row, const int to_col) {
    if (piece->type == PAWN && to_row == board->status.passant_target_row &&
        to_col == board->status.passant_target_col) {
        // El peón enemigo está en la misma columna en la fila de origen
        int captured_pawn_row = (piece->color == WHITE) ? to_row + 1 : to_row - 1;
        Piece *passant_pawn = board->squares[captured_pawn_row][to_col];

        if (passant_pawn && passant_pawn->type == PAWN && passant_pawn->color != piece->color) {
            if (board->status.captured_count < MAX_CAPTURES) {
                board->status.captured_pieces[board->status.captured_count++] = passant_pawn;
                board->squares[captured_pawn_row][to_col] = NULL;

                printf("[DEBUG]: Captura al paso realizada en %c%d\n", 'a' + to_col, 8 - to_row);
                return 1;
            } else {
                printf("Error: Se excedió el límite de piezas capturadas.\n");
                return 0;
            }
        }
        printf("[DEBUG] Intentando comer al paso: peón enemigo en %c%d\n", 'a' + to_col, 8 - captured_pawn_row);
        return 0;
    }
    return 0;
}

// Gestiona el control de poder comer al paso
void reset_peasant(ChessBoard *board, const Piece *piece, const int from_row, const int from_col, const int to_row) {
    // Por defecto, se borra solo si NO es un peón que se mueve dos
    if (piece->type != PAWN || abs(to_row - from_row) != 2) {
        board->status.passant_target_row = -1;
        board->status.passant_target_col = -1;
        return;
    }

    // Movimiento válido de 2 casillas: permite captura al paso
    board->status.passant_target_row = (from_row + to_row) / 2;
    board->status.passant_target_col = from_col;

    printf("[DEBUG] Se habilita captura al paso en %c%d\n", 'a' + board->status.passant_target_col,
           8 - board->status.passant_target_row);
}
