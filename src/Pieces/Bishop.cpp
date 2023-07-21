#include "../../include/Pieces/Bishop.hpp"

// Index-based coordinates constructor
Bishop::Bishop(Team team_, int rank_, int file_):
    Piece(team_, rank_, file_, PieceType::BISHOP, "b")
{
}

// Real coordinates constructor 
Bishop::Bishop(Team team_, coor2dChar& coords_):
    Piece(team_, coords_.first, coords_.second, PieceType::BISHOP, "b")
{
}

std::vector<Move> Bishop::calcPossibleMoves(Board& board_) const
{
    std::vector<Move> moves;
    addDiagonalMovements(board_, moves);
    return moves;
}
