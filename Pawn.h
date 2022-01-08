#pragma once
#include "Piece.h"


// Represents a pawns
struct Pawn : public Piece {
    Pawn(Team, int, int); // Constructor
    void calcPossibleMoves(Piece*[8][8]) const override;
};
