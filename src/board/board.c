#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "board.h"

//TODO: revisar que parametros son const

int piece_from_char(char c, PieceType* type, PieceColor* color) {
    if (c == '-' || c == '\n') return 0;

    *color = isupper(c) ? WHITE : BLACK;
    char upper = toupper(c);

    // Mapeo pizas
    const struct { char symbol; PieceType type; } piece_map[] = {
        {'P', PAWN}, {'R', ROOK}, {'N', KNIGHT}, {'B', BISHOP}, {'Q', QUEEN}, {'K', KING}
    };

    for (size_t i = 0; i < sizeof(piece_map) / sizeof(piece_map[0]); i++) {
        if (piece_map[i].symbol == upper) {
            *type = piece_map[i].type;
            return 1;
        }
    }
    return 0; // No encontrado
}

void initialize_game_status(ChessBoard* board) {
    board->status.captured_count = 0;
    board->status.current_turn = WHITE;
    board->status.move_count = 0;
    board->status.passant_target_row = -1;
    board->status.passant_target_col = -1;
}

void reset_board(ChessBoard* board) {
    initialize_game_status(board);

    // Inicializa el tablero vacío
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            board->squares[row][col] = NULL;
        }
    }
}

void place_piece(ChessBoard* board, int row, int col, PieceType type, PieceColor color) {

    if( (board->squares[row][col] = malloc(sizeof(Piece))) == NULL ){
        fprintf(stderr, "Error: No se pudo asignar memoria para la pieza.\n");
        return; // Sale de la función si no hay memoria disponible
    }
    board->squares[row][col]->type = type;
    board->squares[row][col]->color = color;
}

void initialize_board (ChessBoard* board) {
    // Crea tablero de 0
    reset_board(board);

    // RELLENA EL TABLERO CON PIEZAS
    //Peones
    for (int col = 0; col < BOARD_SIZE; col++) {
        place_piece(board, 6, col, PAWN, WHITE);  // Peones blancos
        place_piece(board, 1, col, PAWN, BLACK);  // Peones negros
    }

    // Torres, caballos y alfiles
    const int piece_positions[3][2] = {
        {0, 7}, // Torres
        {1, 6}, // Caballos
        {2, 5}  // Alfiles
    };

    const PieceType piece_types[3] = {ROOK, KNIGHT, BISHOP};

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            int col = piece_positions[i][j];

            place_piece(board, 7, col, piece_types[i], WHITE);  // Blancas (fila 7)
            place_piece(board, 0, col, piece_types[i], BLACK);  // Negras (fila 0)
        }
    }

    // Reina
    place_piece(board, 7, 3, QUEEN, WHITE);  // Reina blanca (fila 7, columna 3)
    place_piece(board, 0, 3, QUEEN, BLACK);  // Reina negra (fila 0, columna 3)

    // Rey
    place_piece(board, 7, 4, KING, WHITE);  // Rey blanco (fila 7, columna 4)
    place_piece(board, 0, 4, KING, BLACK);  // Rey negro (fila 0, columna 4)
}

void initialize_custom_board_from_file(ChessBoard* board, const char *filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: No se pudo abrir el archivo %s\n", filename);
        return;
    }

    reset_board(board);

    char line[BOARD_SIZE + 2];  // +2 para '\n' y '\0'
    int row = 0;

    while (fgets(line, sizeof(line), file) && row < BOARD_SIZE) {
        for (int col = 0; col < BOARD_SIZE && line[col] != '\n' && line[col] != '\0'; col++) {
            PieceType type;
            PieceColor color;
    
            if (!piece_from_char(line[col], &type, &color)) {
                if (line[col] != '-') {
                    fprintf(stderr, "Caracter inválido '%c' en fila %d, columna %d\n", line[col], row, col);
                }
                continue; // Casilla vacía o carácter inválido
            }
    
            place_piece(board, row, col, type, color);
        }
        row++;
    }
    fclose(file);
}

void free_board(ChessBoard* board) {
    if (!board) return;

    // LIBERA TABLERO
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (board->squares[row][col] != NULL) {
                free(board->squares[row][col]);  // Liberar memoria de la pieza
                board->squares[row][col] = NULL; // Evitar accesos inválidos
            }
        }
    }

    // LIBERA FICHAS CAPTURADAS
    for (int i = 0; i < board->status.captured_count; i++) {
        if (board->status.captured_pieces[i] != NULL) {
            free(board->status.captured_pieces[i]);  // Liberar la memoria de la pieza capturada
            board->status.captured_pieces[i] = NULL; // Evitar accesos inválidos
        }
    }
    
    // Reiniciar el contador de piezas capturadas
    board->status.captured_count = 0;

    free(board);
}