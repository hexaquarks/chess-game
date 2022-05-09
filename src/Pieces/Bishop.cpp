#include "../../include/Pieces/Bishop.hpp"

Bishop::Bishop(Team team_, int x_, int y_):
Piece(team_, x_, y_, PieceType::BISHOP, "b") 
{
}

vector<Move> Bishop::calcPossibleMoves(shared_ptr<Piece> board_[8][8]) const
{
    return getDiagonalMovements(board_);
}
