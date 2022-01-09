#pragma once
#include "Piece.h"
#include <vector>


// Represents a pawns
class Pawn : public Piece {
    bool hasMoved = false; // First move can eventually 2

    public:
    Pawn(Team, int, int); // Constructor
    moveTypes calcPossibleMoves(Piece*[8][8]) const override;
};
