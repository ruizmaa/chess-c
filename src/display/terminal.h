#ifndef TERMINAL_H
#define TERMINAL_H

// COLORES
#define RESET "\x1B[0m"

// CLASICO
//#define WHITE_COLOR  "\x1B[97m"
//#define BLACK_COLOR  "\x1B[30m"

//#define BG_LIGHT "\x1B[48;5;255m"
//#define BG_DARK  "\x1B[48;5;232m"

// CONTRASTE
#define RESET       "\x1B[0m"
#define WHITE_COLOR "\x1B[38;5;46m"  // Verde brillante para piezas blancas
#define BLACK_COLOR "\x1B[38;5;198m" // Rosa brillante para piezas negras

#define BG_LIGHT "\x1B[48;5;234m" // Gris oscuro para casillas claras
#define BG_DARK  "\x1B[48;5;0m"

void print_piece(Piece *p, int use_unicode);
void print_captured_pieces(ChessBoard *board, int use_unicode);
void print_board(ChessBoard *board, int use_unicode);

#endif