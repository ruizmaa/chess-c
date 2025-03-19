#ifndef BOARD_H
#define BOARD_H

#define BOARD_SIZE 8
#define MAX_CAPTURES 32

typedef enum {
    PAWN,
    ROOK,
    KNIGHT,
    BISHOP,
    QUEEN,
    KING
} PieceType;

typedef enum {
    WHITE,
    BLACK
} PieceColor;

typedef struct {
    PieceType type;
    PieceColor color; 
} Piece;

typedef struct {
    // Piezas capturadas
    Piece* captured_pieces[MAX_CAPTURES];
    int captured_count;

    // Turno y n movimientos
    PieceColor current_turn;
    int move_count;

    // Captura al paso
    int passant_target_row;   
    int passant_target_col; 
} GameStatus;

typedef struct {
    Piece* squares [BOARD_SIZE][BOARD_SIZE];
    GameStatus status;
} ChessBoard;


//TODO: revisar los cons
void place_piece(ChessBoard* board, int row, int col, PieceType type, PieceColor color);
void initialize_game_status(ChessBoard* board);
void initialize_board (ChessBoard* board);
//TODO: revisar que se aplique el free a todo ChessBoard en free_board()
void free_board(ChessBoard* board);
void initialize_custom_board(ChessBoard* board);
void load_board_from_file(const char *filename, char custom_board[BOARD_SIZE][BOARD_SIZE]);

#endif