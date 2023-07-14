#include "../../include/Pieces/Rook.hpp"

#include <vector>

Rook::Rook(Team team_, int rank_, int file_)
: Piece(team_, rank_, file_, PieceType::ROOK, "r")
{
}

std::vector<Move> Rook::calcPossibleMoves(Board& board_) const
{
    std::vector<Move> moves;
    addHorizontalAndVerticalMovements(board_, moves);
    return moves;
}
