#pragma once
#include "Piece.h"
#include <vector>


// Represents a pawns
struct Pawn : public Piece {
    Pawn(Team, int, int); // Constructor
    moveTypes calcPossibleMoves(Piece*[8][8]) const override;
};
