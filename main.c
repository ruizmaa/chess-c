#include "board.h"
#include "game.h"
#include <stdio.h>

int main() {
    // Creacion del tablero
    ChessBoard board;
    // initialize_board(&board);

    initialize_custom_board_from_file(&board, "src/tests/castling/long_black_ok.txt");

    while (1) {
        play_turn(&board);
    }

    return 0;
}
