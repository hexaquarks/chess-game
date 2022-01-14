#pragma once
#include "Piece.hpp"
#include "Pawn.hpp"
#include "Rook.hpp"
#include "Knight.hpp"
#include "Bishop.hpp"
#include "King.hpp"
#include "Queen.hpp"

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

    bool kingIsChecked() { 
        return (turn == Team::WHITE)? whiteKing->isChecked(board): blackKing->isChecked(board); 
    }

    moveTypes possibleMovesFor(Piece* piece) { return piece->calcPossibleMoves(board); }

    void addPiece(Piece* piece) { 
        (piece->getTeam() == Team::WHITE) ? whitePieces.push_back(piece) : blackPieces.push_back(piece); 
    }

    void applyMove(moveType* selectedMove, int xPos, int yPos, Piece* selectedPiece, Piece* lastMove, int CELL_SIZE) ;
    void revertMove() const;
};
