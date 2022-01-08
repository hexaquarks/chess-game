#pragma once
#include "Piece.h"
#include <vector>


// Represents a pawns
struct Pawn : public Piece {
    Pawn(Team, int, int); // Constructor
    vector<tuple<pair<int, int> , MoveType>> calcPossibleMoves(Piece*[8][8]) const override;
    bool hasMoved;
};
