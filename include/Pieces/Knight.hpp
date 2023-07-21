#pragma once
#include "Piece.hpp"

// Represents a knight
struct Knight: public Piece
{
    Knight(Team, int, int); // index-based
    Knight(Team, int, char); // real coordinates

    std::vector<Move> calcPossibleMoves(Board&) const override;
};
