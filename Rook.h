#pragma once
#include "Piece.h"


// Represents a rook
class Rook : public Piece {
    bool hasMoved = false; // Useful for castling

    public:
    Rook(Team, int, int); // Constructor
    moveTypes calcPossibleMoves(Piece*[8][8]) const override;
};
