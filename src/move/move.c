#include "move.h"
#include "validate.h"
#include <stdio.h>
#include <stdlib.h>

// Función para mover una pieza
int move_piece(ChessBoard* board, const int from_row, const int from_col, const int to_row, const int to_col) {
    Piece *piece = board->squares[from_row][from_col];

    // No hay pieza en la posicion de origen
    if (!piece) return 0;

    // Comprueba que el movimiento sea valido
    if (!is_valid_move(board, from_row, from_col, to_row, to_col)) return 0;

    Piece *captured_piece = board->squares[to_row][to_col];

    // Si come pieza se anade al game status
    if (captured_piece != NULL) {
        //free(board->squares[to_row][to_col]);
        if (board->status.captured_count < MAX_CAPTURES) {
            board->status.captured_pieces[board->status.captured_count++] = captured_piece;  // Guardar pieza capturada
        } else {
            printf("Error: Se excedió el límite de piezas capturadas.\n");
            return 0;
        }
    }

    board->squares[to_row][to_col] = piece;
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