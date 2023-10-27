#include "../../../include/Logic/Pieces/Rook.hpp"

#include <vector>

// Index-based coordinates constructor
Rook::Rook(Team team_, int file_, int rank_)
: Piece(team_, file_, rank_, PieceType::ROOK, "r")
{
}

// Real coordinates constructor
Rook::Rook(Team team_, coor2dChar& coords_)
: Piece(team_, coords_.first, coords_.second, PieceType::ROOK, "r")
{
}

std::vector<Move> Rook::calcPossibleMoves(Board& board_) const
{
    std::vector<Move> moves;
    addHorizontalAndVerticalMovements(board_, moves);
    return moves;
}
