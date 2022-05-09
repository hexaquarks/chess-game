#include "../../include/Pieces/Queen.hpp"
#include <vector>

Queen::Queen(Team team_, int x_, int y_): Piece(team_, x_, y_, PieceType::QUEEN, "q") 
{
}

vector<Move> Queen::calcPossibleMoves(Piece* pBoard_[8][8]) const 
{
    // Combine horizontal, vertical and diagonal movements
    vector<Move> moves = getHorizontalAndVerticalMovements(pBoard_);
    for (auto& move: getDiagonalMovements(pBoard_))
        moves.push_back(move);
    return moves;
}
