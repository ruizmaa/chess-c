#include <stdio.h>

#include "board.h"
#include "game.h"
#include "input.h"
#include "move.h"
#include "terminal.h"

// Manejar el turno del jugador
void play_turn(ChessBoard *board) {
    // ['a', '2', '/0']
    char from[3], to[3];
    int from_row, from_col, to_row, to_col;
    Piece *selected_piece;

    // Turno y color para la terminal
    PieceColor turn;
    char *turn_color;

    while (1) {
        turn = board->status.current_turn;
        turn_color = (board->status.current_turn == WHITE) ? WHITE_COLOR : BLACK_COLOR;

        print_board(board, 1);

        // Printeo inicio del turno
        printf("%sTurno de las %s.%s\n", turn_color, turn == WHITE ? "blancas" : "negras", RESET);
        printf("Selecciona una pieza para mover (ej: e2): ");

        get_user_input(from);                        // Pido un input valido
        get_coordinates(from, &from_row, &from_col); // Convertir la entrada en coordenadas
        selected_piece = board->squares[from_row][from_col];
        if (!is_valid_from_piece(selected_piece, turn, from))
            continue; // Validar que la coordenada sea válida (haya una pieza del jugador)

        // Printeo destino
        printf("Selecciona la casilla de destino (ej: e4): ");
        get_user_input(to);                    // Pido un input valido
        get_coordinates(to, &to_row, &to_col); // Convertir la entrada en coordenadas

        // Muevo la ficha
        if (move_piece(board, from_row, from_col, to_row, to_col)) {
            change_turn(board);
        } else {
            printf("Movimiento inválido. Intenta de nuevo.\n");
        }
    }
}

void change_turn(ChessBoard *board) {
    if (board->status.current_turn == BLACK)
        board->status.current_turn = WHITE;
    else
        board->status.current_turn = BLACK;
    board->status.move_count++;
}