#include "board.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    // Creacion del tablero
    ChessBoard *board = malloc(sizeof(ChessBoard));
    if (!board) {
        fprintf(stderr, "Error al asignar memoria\n");
        return 1;
    }

    initialize_board(board);
    // initialize_custom_board_from_file(&board, "src/tests/check.txt");

    while (1) {
        play_turn(board);
    }

    free_board(board);
    return 0;
}
