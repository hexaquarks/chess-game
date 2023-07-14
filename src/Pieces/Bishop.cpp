#include "../../include/Pieces/Bishop.hpp"

Bishop::Bishop(Team team_, int rank_, int file_):
    Piece(
        team_, 
        rank_, 
        file_, 
        PieceType::BISHOP, "b")
{
}

std::vector<Move> Bishop::calcPossibleMoves(Board& board_) const
{
    std::vector<Move> moves;
    addDiagonalMovements(board_, moves);
    return moves;
}
