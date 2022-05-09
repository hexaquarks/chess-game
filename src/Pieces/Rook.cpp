#include "../../include/Pieces/Rook.hpp"
#include <vector>

Rook::Rook(Team team_, int x_, int y_)
: Piece(team_, x_, y_, PieceType::ROOK, "r") 
{
}

vector<Move> Rook::calcPossibleMoves(shared_ptr<Piece> pBoard_[8][8]) const 
{
    return getHorizontalAndVerticalMovements(pBoard_);
}
