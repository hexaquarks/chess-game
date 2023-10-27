#pragma once
#include "Piece.hpp"

// Represents a bishop
struct Bishop: public Piece
{
    explicit Bishop(Team, int, int); // Index-based constructor
    explicit Bishop(Team, coor2dChar&); // real coordinates

    std::vector<Move> calcPossibleMoves(Board& board_) const override;
};
