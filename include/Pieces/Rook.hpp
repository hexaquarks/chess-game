#pragma once
#include "Piece.hpp"

// Represents a rook
struct Rook: public Piece
{
    explicit Rook(Team, int, int); // index-based
    explicit Rook(Team, coor2dChar&); // real coordinates

    std::vector<Move> calcPossibleMoves(Board&) const override;
};
