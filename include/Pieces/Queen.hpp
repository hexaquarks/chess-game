#pragma once
#include "Piece.hpp"

// Represents a queen
struct Queen: public Piece
{
    Queen(Team, int, int); // index-based
    Queen(Team, int, char); // real coordinates

    std::vector<Move> calcPossibleMoves(Board&) const override;
};
