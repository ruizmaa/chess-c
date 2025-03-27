#include <stdio.h>

#include "board.h"
#include "terminal.h"

static const char *PIECE_UNICODE[2][6] = {
    {"♙", "♖", "♘", "♗", "♕", "♔"}, // Blancas
    {"♟", "♜", "♞", "♝", "♛", "♚"}  // Negras
};

static const char PIECE_ASCII[6] = {'P', 'R', 'N', 'B', 'Q', 'K'};

void print_piece(Piece *p, int use_unicode) {
    if (p == NULL) {
        printf("   "); // Casilla vacía
        return;
    }

    char *text_color = (p->color == WHITE) ? WHITE_COLOR : BLACK_COLOR;

    if (use_unicode) {
        printf("%s %s " RESET, text_color, PIECE_UNICODE[p->color][p->type]);
    } else {
        printf("%s %c " RESET, text_color, PIECE_ASCII[p->type]);
    }
}

void print_captured_pieces(ChessBoard *board, int use_unicode) {
    printf("\nPiezas capturadas:\n");

    int count = 0;            // Contador para controlar el ancho
    int pieces_per_line = 10; // Número de piezas mostradas por cada línea

    for (int i = 0; i < board->status.captured_count; i++) {
        Piece *cp = board->status.captured_pieces[i];
        if (cp != NULL) {
            char *text_color = (cp->color == WHITE) ? WHITE_COLOR : BLACK_COLOR;

            if (use_unicode) {
                printf("%s%s " RESET, text_color, PIECE_UNICODE[cp->color][cp->type]);
            } else {
                printf("%s%c " RESET, text_color, PIECE_ASCII[cp->type]);
            }
            count++;

            // Si se imprimieron 10 piezas, saltar a la siguiente línea
            if (count % pieces_per_line == 0) { printf("\n"); }
        }
    }

    printf("\n"); // Asegurar que la última línea termine bien
}

// TODO: hacer que se muestren las capturadas al lado del tablero
void print_board(ChessBoard *board, int use_unicode) {
    // Printeo del tablero
    printf("\n   a  b  c  d  e  f  g  h\n");
    for (int row = 0; row < BOARD_SIZE; row++) {
        printf("%d ", 8 - row);
        for (int col = 0; col < BOARD_SIZE; col++) {
            // Alternar colores de fondo según la casilla
            char *bg_color = ((row + col) % 2 == 0) ? BG_LIGHT : BG_DARK;

            printf("%s", bg_color); // Aplicar color de fondo
            print_piece(board->squares[row][col], use_unicode);
            printf(RESET);
        }
        printf("\n");
    }
    printf("\n");
    print_captured_pieces(board, use_unicode);
}