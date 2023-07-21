#include "../../include/Pieces/Queen.hpp"

#include <vector>

// Index-based coordinates constructor
Queen::Queen(Team team_, int rank_, int file_): 
    Piece(team_, rank_, file_, PieceType::QUEEN, "q")
{
}

// Real coordinates constructor
Queen::Queen(Team team_, int rank_, char file_): 
    Piece(team_, rank_, file_, PieceType::QUEEN, "q")
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
