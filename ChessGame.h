#pragma once
#include "Piece.h"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "King.h"
#include "Queen.h"


class ChessGame {
    Piece* board[8][8];
    vector<Piece*> whitePieces;
    vector<Piece*> blackPieces;
    King* whiteKing;
    King* blackKing;
    void freeMemory();

    public:
    ChessGame() { reset(); } // Constructor
    ~ChessGame() { freeMemory(); } // Destructor
    
    void reset(); // Resets the board
    Piece* getBoardTile(int x, int y) { return board[x][y]; }
    void setBoardTile(int, int, Piece*);
};
