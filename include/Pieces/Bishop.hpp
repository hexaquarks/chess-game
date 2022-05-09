#pragma once
#include "Piece.hpp"


// Represents a bishop
struct Bishop: public Piece
{
    Bishop(Team, int, int); // Constructor
    vector<Move> calcPossibleMoves(shared_ptr<Piece>[8][8]) const override;
};
