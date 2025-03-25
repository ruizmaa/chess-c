#include <stdio.h>
#include "board.h"
#include "game.h"

int main() {
    // Creacion del tablero
    ChessBoard board;
    //initialize_board(&board);

    initialize_custom_board_from_file(&board, "src/tests/rook.txt");

    while (1) {
        play_turn(&board);
    }

    return 0;
}
