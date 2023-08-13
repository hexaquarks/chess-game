#include "../../include/Pieces/Queen.hpp"

#include <vector>

// Index-based coordinates constructor
Queen::Queen(Team team_, int file_, int rank_): 
    Piece(team_, file_, rank_, PieceType::QUEEN, "q")
{
}

// Real coordinates constructor
Queen::Queen(Team team_, coor2dChar& coords_): 
    Piece(team_, coords_.first, coords_.second, PieceType::QUEEN, "q")
{
}

std::vector<Move> Queen::calcPossibleMoves(Board& board_) const
{
    // Combine horizontal, vertical and diagonal movements
    std::vector<Move> moves;
    addHorizontalAndVerticalMovements(board_, moves);
    addDiagonalMovements(board_, moves);
    return moves;
}
