#ifndef VALIDATE_H
#define VALIDATE_H

#include "board.h"

int is_friendly_occupied(const PieceColor color_turn, const Piece *destination);

int is_valid_move(const ChessBoard *board, const int from_row, const int from_col, const int to_row, const int to_col);

int is_valid_pawn_move(const ChessBoard *board, const int from_row, const int from_col, const int to_row,
                       const int to_col);
int is_valid_rook_move(const ChessBoard *board, const int from_row, const int from_col, const int to_row,
                       const int to_col);
int is_valid_knight_move(const int from_row, const int from_col, const int to_row, const int to_col);
int is_valid_bishop_move(const ChessBoard *board, const int from_row, const int from_col, const int to_row,
                         const int to_col);
int is_valid_queen_move(const ChessBoard *board, const int from_row, const int from_col, const int to_row,
                        const int to_col);
int is_valid_king_move(const int from_row, const int from_col, const int to_row, const int to_col);
int is_valid_castling_move(const ChessBoard *board, int from_row, int from_col, int to_row, int to_col);
int is_square_attacked(const ChessBoard *board, const int row, const int col, const PieceColor by_color);
int would_cause_check(ChessBoard *board, const int from_row, const int from_col, const int to_row, int to_col,
                      const PieceColor color);

#endif
