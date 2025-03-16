#ifndef GAME_H
#define GAME_H

#include "board.h"

// Registro de piezas comidas
typedef struct {
    Piece* captured_pieces[32];
    int captured_count;
} GameStatus;

void play_turn(ChessBoard* board, const PieceColor current_turn);
int move_piece(ChessBoard* board, const int from_row, const int from_col, const int to_row, const int to_col);
void get_coordinates(const char *input, int *row, int *col);
void get_user_input(char *input);
int is_valid_input(const char *input);
int is_valid_from_piece(const Piece *selected_piece, const PieceColor current_turn, const char *from);

#endif
