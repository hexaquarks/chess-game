#include "Piece.h"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "King.h"
#include "Queen.h"

class ChessGame {
    public:
    ChessGame(); //constructor
    ~ChessGame(); //destructor 

    Piece* board[8][8];

    Pawn* pw1;
    Pawn* pw2;
    Pawn* pw3;
    Pawn* pw4;
    Pawn* pw5;
    Pawn* pw6;
    Pawn* pw7;
    Pawn* pw8;
    Pawn* pb1;
    Pawn* pb2;
    Pawn* pb3;
    Pawn* pb4;
    Pawn* pb5;
    Pawn* pb6;
    Pawn* pb7;
    Pawn* pb8;
    Rook* rb1;
    Rook* rb2;
    Rook* rw1;
    Rook* rw2;
    Knight* nb1;
    Knight* nb2;
    Knight* nw1;
    Knight* nw2;
    Bishop* bb1;
    Bishop* bb2;
    Bishop* bw1;
    Bishop* bw2;
    King* kb1;
    King* kw1;
    Queen* qb1; 
    Queen* qw1; 
};