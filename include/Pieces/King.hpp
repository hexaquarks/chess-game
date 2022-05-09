#pragma once
#include "Piece.hpp"


// Represents a king
class King: public Piece 
{
    vector<Move> possibleMovesNoCheck(Piece*[8][8]) const;
    bool isChecked(Piece*[8][8], int, int) const;
    bool canCastleKingSide(Piece*[8][8]) const;
    bool canCastleQueenSide(Piece*[8][8]) const;

    public:
    King(Team, int, int); // Constructor
    vector<Move> calcPossibleMoves(Piece*[8][8]) const override;
    bool isChecked(Piece* board[8][8]) const { return isChecked(board, getX(), getY()); }
    
    static void swapPieces(Piece* board[8][8], int x, int y, int X, int Y) 
    {
        Piece* first = board[x][y];
        board[x][y] = board[X][Y];
        board[X][Y] = first;
    }
};
