#pragma once
#include "Piece.hpp"

// Represents a rook
struct Rook: public Piece
{
    Rook(Team, int, int); // index-based
    Rook(Team, int, char); // real coordinates

    std::vector<Move> calcPossibleMoves(Board&) const override;
};
