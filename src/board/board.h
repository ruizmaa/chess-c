#ifndef BOARD_H
#define BOARD_H

#define BOARD_SIZE 8
#define MAX_CAPTURES 32

//COLORES
#define RESET  "\x1B[0m"

//CLASICO
//#define WHITE_COLOR  "\x1B[97m"
//#define BLACK_COLOR  "\x1B[30m"

//#define BG_LIGHT "\x1B[48;5;255m"
//#define BG_DARK  "\x1B[48;5;232m"

//CONTRASTE
#define RESET  "\x1B[0m"
#define WHITE_COLOR  "\x1B[38;5;46m"   // Verde brillante para piezas blancas
#define BLACK_COLOR  "\x1B[38;5;198m"  // Rosa brillante para piezas negras

#define BG_LIGHT "\x1B[48;5;234m"  // Gris oscuro para casillas claras
#define BG_DARK  "\x1B[48;5;0m"

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
void free_board(ChessBoard* board);
void initialize_custom_board(ChessBoard* board);
void print_piece(Piece *p, int use_unicode);
void print_captured_pieces(ChessBoard* board, int use_unicode);
void print_board(ChessBoard* board, int use_unicode);

#endif