#pragma once
#include "Piece.hpp"

// Represents a bishop
struct Bishop: public Piece
{
    Bishop(Team, int, int); // Constructor
    std::vector<Move> calcPossibleMoves(Board& board_) const override;
};
