#pragma once
#include "Piece.h"


// Represents a king
class King : public Piece {
    bool hasMoved = false; // Useful for castling

    public:
    King(Team, int, int); // Constructor
    moveTypes calcPossibleMoves(Piece*[8][8]) const override; 
};
