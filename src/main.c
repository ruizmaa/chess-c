#include <stdio.h>
#include "board.h"



int main() {
    ChessBoard board;
    inicialize_board(&board);
    print_board(&board, 1);
    free_board(&board);  // Liberamos memoria antes de salir

    return 0;
}
