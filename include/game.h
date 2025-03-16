#ifndef GAME_H
#define GAME_H

#include "board.h"

void play_turn(ChessBoard* board, PieceColor current_turn);
int move_piece(ChessBoard* board, int from_row, int from_col, int to_row, int to_col);
void get_coordinates(const char *input, int *row, int *col);
void get_user_input(char *input);
int is_valid_input(const char *input);
int is_valid_from_piece(const Piece *selected_piece, const PieceColor current_turn, const char *from);

#endif
