#ifndef MOVE_H
#define MOVE_H

#include "board.h"

int move_piece(ChessBoard *board, const int from_row, const int from_col, const int to_row, const int to_col);
int is_valid_from_piece(const Piece *selected_piece, const PieceColor current_turn, const char *from);
int take_piece(ChessBoard *board, Piece *piece, const int to_row, const int to_col);
int take_piece_peasant(ChessBoard *board, Piece *piece, const int to_row, const int to_col);
void reset_peasant(ChessBoard *board, const Piece *piece, const int from_row, const int from_col, const int to_row);

#endif
