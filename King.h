#pragma once
#include "Piece.h"


// Represents a king
class King : public Piece {
    moveTypes possibleMovesNoCheck(Piece*[8][8]) const;
    bool isChecked(Piece*[8][8], int, int) const;

    public:
    King(Team, int, int); // Constructor
    moveTypes calcPossibleMoves(Piece*[8][8]) const override;
    bool isChecked(Piece* board[8][8]) const { return isChecked(board, getX(), getY()); }
};
