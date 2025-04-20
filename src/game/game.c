#include <stdio.h>
#include <string.h>

#include "board.h"
#include "game.h"
#include "input.h"
#include "move.h"
#include "terminal.h"
#include "validate.h"

// Manejar el turno del jugador
void play_turn(ChessBoard *board) {
    // ['a', '2', '/0']
    char from[3], to[3];
    int from_row, from_col, to_row, to_col;
    Piece *selected_piece;
    char input[5];

    // Turno y color para la terminal
    PieceColor turn;
    char *turn_color;

    while (1) {
        turn = board->status.current_turn;
        turn_color = (board->status.current_turn == WHITE) ? WHITE_COLOR : BLACK_COLOR;

        print_board(board, 1);

        // Verificar si el jugador esta en jaque
        if (is_in_check(board, turn)) {
            printf("%sEl rey %s está en jaque%s\n", turn_color, (turn == WHITE ? "blanco" : "negro"), RESET);
        }

        // Printeo inicio del turno
        printf("%sTurno de las %s.%s\n", turn_color, turn == WHITE ? "blancas" : "negras", RESET);
        printf("Selecciona una pieza para mover (ej: e2): ");

        get_user_input(input);

        // El jugador metió ambas coordenadas: ej. b5b6
        if (strlen(input) == 4) {
            strncpy(from, input, 2);
            from[2] = '\0';
            strncpy(to, input + 2, 2);
            to[2] = '\0';
        } else {
            strncpy(from, input, 3);
            printf("Selecciona la casilla de destino (ej: e4): ");
            get_user_input(to);
        }

        // Convertir la entrada en coordenadas
        get_coordinates(from, &from_row, &from_col);
        get_coordinates(to, &to_row, &to_col);

        selected_piece = board->squares[from_row][from_col];
        if (!is_valid_from_piece(selected_piece, turn, from))
            continue; // Validar que la coordenada sea válida (haya una pieza del jugador)

        // Muevo la ficha
        if (move_piece(board, from_row, from_col, to_row, to_col)) { change_turn(board); }
    }
}

void change_turn(ChessBoard *board) {
    if (board->status.current_turn == BLACK)
        board->status.current_turn = WHITE;
    else
        board->status.current_turn = BLACK;
    board->status.move_count++;
}