#pragma once
#include "Piece.h"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "King.h"
#include "Queen.h"

class ChessGame {
    vector<Piece*> whitePieces;
    vector<Piece*> blackPieces;
    King* whiteKing;
    King* blackKing;
    void freeMemory();

    public:
    Piece* board[8][8];
    ChessGame() { reset(); } // Constructor
    ~ChessGame() { freeMemory(); } // Destructor
    
    Team turn; // which player's turn
    void reset(); // Resets the board
    Piece* getBoardTile(int x, int y) { return board[x][y]; }
    void setBoardTile(int, int, Piece*);
    Team getTurn() { return turn; }
};
