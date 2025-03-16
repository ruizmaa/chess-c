#include <stdio.h>
#include "board.h"
#include "game.h"

int main() {
    // Creacion del tablero
    ChessBoard board;
    initialize_board(&board);

    // Turno 0
    PieceColor current_turn = WHITE;

    while (1) {
        print_board(&board, 1);
        play_turn(&board, current_turn);

        // Cambiar de turno
        current_turn = (current_turn == WHITE) ? BLACK : WHITE;
    }

    return 0;
}
