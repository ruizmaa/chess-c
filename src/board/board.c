#include <stdio.h>
#include <stdlib.h>
#include "board.h"

//TOD: revisar que parametros son const

void place_piece(ChessBoard* board, int row, int col, PieceType type, PieceColor color) {

    if( (board->squares[row][col] = malloc(sizeof(Piece))) == NULL ){
        fprintf(stderr, "Error: No se pudo asignar memoria para la pieza.\n");
        return; // Sale de la función si no hay memoria disponible
    }
    board->squares[row][col]->type = type;
    board->squares[row][col]->color = color;
}

void initialize_game_status(ChessBoard* board) {
    board->status.captured_count = 0;
    board->status.current_turn = WHITE;
    board->status.move_count = 0;
    board->status.passant_target_row = -1;
    board->status.passant_target_col = -1;
}

void initialize_board (ChessBoard* board){
    // INICIALIZA GAME STATUS
    initialize_game_status(board);

    // CREA EL TABLERO VACIO
    for (int row = 0; row < BOARD_SIZE; row++)
    {
       for (int col = 0; col < BOARD_SIZE; col++){
            board->squares[row][col] = NULL;
       }
    }

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

void initialize_custom_board(ChessBoard* board) {
    // INICIALIZA GAME STATUS
    initialize_game_status(board);

    board->status.captured_count = 30;

    for (int i = 0; i < 20; i++) {
        Piece *captured_piece = malloc(sizeof(Piece)); // Asigna memoria para la pieza capturada
        captured_piece->type = PAWN;
        captured_piece->color = WHITE;
        board->status.captured_pieces[i] = captured_piece; // Guarda la pieza en el arreglo de capturadas
    }

    for (int i = 20; i < 30; i++) {
        Piece *captured_piece = malloc(sizeof(Piece)); // Asigna memoria para la pieza capturada
        captured_piece->type = PAWN;
        captured_piece->color = BLACK;
        board->status.captured_pieces[i] = captured_piece; // Guarda la pieza en el arreglo de capturadas
    }

    const char custom_board[BOARD_SIZE][BOARD_SIZE] = {
        {'-', '-', '-', '-', '-', '-', '-', '-'},
        {'-', '-', '-', '-', '-', '-', '-', '-'},
        {'-', '-', '-', '-', '-', '-', '-', '-'},
        {'-', '-', '-', '-', '-', '-', '-', '-'},
        {'R', '-', 'r', '-', 'b', '-', '-', '-'},
        {'-', '-', '-', '-', '-', '-', '-', '-'},
        {'-', '-', '-', '-', '-', '-', '-', '-'},
        {'R', '-', 'q', '-', '-', '-', '-', '-'}
    };

    // Mapeo pizas
    const struct { char symbol; PieceType type; } piece_map[] = {
        {'P', PAWN}, {'R', ROOK}, {'N', KNIGHT}, {'B', BISHOP}, {'Q', QUEEN}, {'K', KING}
    };

    //CREA EL TABLERO VACIO
    for (int row = 0; row < BOARD_SIZE; row++)
    {
        for (int col = 0; col < BOARD_SIZE; col++){
            board->squares[row][col] = NULL;
        }
    }

    //COLOCAR PIEZAS
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            char c = custom_board[row][col];
            PieceColor color = (c >= 'A' && c <= 'Z') ? WHITE : BLACK;  // Mayúsculas = Blancas, Minúsculas = Negras
            if (c == '-') continue;  // Espacio vacío

            // Buscar el tipo de pieza
            for (int i = 0; i < 6; i++) {
                if (c == piece_map[i].symbol || c == (piece_map[i].symbol + ('a' - 'A'))) {
                    place_piece(board, row, col, piece_map[i].type, color);
                    break;
                }
            }
        }
    }
}
