#include "../../include/Pieces/Rook.hpp"

#include <vector>

Rook::Rook(Team team_, int x_, int y_)
: Piece(team_, x_, y_, PieceType::ROOK, "r")
{
}

vector<Move> Rook::calcPossibleMoves(Board& board_) const
{
    vector<Move> moves;
    addHorizontalAndVerticalMovements(board_, moves);
    return moves;
}
