#pragma once
#include "Piece.hpp"


// Represents a king
class King: public Piece
{
    vector<Move> possibleMovesNoCheck(shared_ptr<Piece>[8][8]) const;
    bool isChecked(shared_ptr<Piece>[8][8], int, int) const;
    bool canCastleKingSide(shared_ptr<Piece>[8][8]) const;
    bool canCastleQueenSide(shared_ptr<Piece>[8][8]) const;

    public:
    King(Team, int, int); // Constructor
    vector<Move> calcPossibleMoves(shared_ptr<Piece>[8][8]) const override;
    bool isChecked(shared_ptr<Piece> board[8][8]) const { return isChecked(board, getX(), getY()); }
   
    static void swapPieces(shared_ptr<Piece> board[8][8], int x, int y, int X, int Y)
    {
        shared_ptr<Piece> first = board[x][y];
        board[x][y] = board[X][Y];
        board[X][Y] = first;
    }
};
