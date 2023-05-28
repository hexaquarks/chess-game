#pragma once
#include "Piece.hpp"

// Represents a queen
struct Queen: public Piece
{
    Queen(Team, int, int); // Constructor
    std::vector<Move> calcPossibleMoves(Board&) const override;
};
