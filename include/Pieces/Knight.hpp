#pragma once
#include "Piece.hpp"

// Represents a knight
struct Knight: public Piece
{
    explicit Knight(Team, int, int); // index-based
    explicit Knight(Team, coor2dChar&); // real coordinates

    std::vector<Move> calcPossibleMoves(Board&) const override;
};
