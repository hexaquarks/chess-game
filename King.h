#pragma once
#include "Piece.h"


// Represents a king
class King : public Piece {
    moveTypes possibleMovesNoCheck(Piece*[8][8]) const;

    public:
    King(Team, int, int); // Constructor
    moveTypes calcPossibleMoves(Piece*[8][8]) const override; 
};
