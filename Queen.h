#pragma once
#include "Piece.h"


// Represents a queen
struct Queen : public Piece {
    Queen(Team, int, int); // Constructor
    void calcPossibleMoves(Piece*s[8][8]) const override;
};
