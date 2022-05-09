#pragma once
#include "Piece.hpp"


// Represents a rook
struct Rook: public Piece 
{
    Rook(Team, int, int); // Constructor
    vector<Move> calcPossibleMoves(shared_ptr<Piece>[8][8]) const override;
};
