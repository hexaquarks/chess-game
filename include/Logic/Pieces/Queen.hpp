#pragma once
#include "Piece.hpp"

// Represents a queen
struct Queen: public Piece
{
    explicit Queen(Team, int, int); // index-based
    explicit Queen(Team, coor2dChar&); // real coordinates

    std::vector<Move> calcPossibleMoves(Board&) const override;
};
