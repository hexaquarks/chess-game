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
    Team turn; // which player's turn
    vector<Piece*> whitePieces;
    vector<Piece*> blackPieces;
    King* whiteKing;
    King* blackKing;
    void freeMemory();

    public:
    ChessGame(): turn(Team::WHITE) { reset(); } // Constructor
    ~ChessGame() { freeMemory(); } // Destructor

    void reset(); // Resets the board
    Piece* getBoardTile(int x, int y) const { return board[y][x]; }
    void setBoardTile(int, int, Piece*);
    Team getTurn() const { return turn; }
    void switchTurn() { turn = (turn == Team::WHITE)? Team::BLACK: Team::WHITE; }
    moveTypes possibleMovesFor(Piece* piece) { return piece->calcPossibleMoves(board); }
};
