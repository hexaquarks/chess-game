#pragma once
#include "Piece.hpp"

// Represents a bishop
struct Bishop: public Piece
{
    Bishop(Team, int, int); // Index-based constructor
    Bishop(Team, int, char); // real coordinates
    
    std::vector<Move> calcPossibleMoves(Board& board_) const override;
};
