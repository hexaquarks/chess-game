#include "../../include/Pieces/Rook.hpp"

#include <vector>

// Index-based coordinates constructor
Rook::Rook(Team team_, int rank_, int file_)
: Piece(team_, rank_, file_, PieceType::ROOK, "r")
{
}

// Real coordinates constructor
Rook::Rook(Team team_, int rank_, char file_)
: Piece(team_, rank_, file_, PieceType::ROOK, "r")
{
}

std::vector<Move> Rook::calcPossibleMoves(Board& board_) const
{
    std::vector<Move> moves;
    addHorizontalAndVerticalMovements(board_, moves);
    return moves;
}
