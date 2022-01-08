#pragma once
#include "Piece.h"


// Represents a knight
struct Knight : public Piece {
    Knight(Team, int, int); // Constructor
    vector<tuple<pair<int, int> , MoveType>> calcPossibleMoves(Piece*[8][8]) const override;
};
