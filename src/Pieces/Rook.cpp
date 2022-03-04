#include "../../include/Pieces/Rook.hpp"
#include <vector>

Rook::Rook(Team team, int x, int y): Piece(team, x, y, PieceType::ROOK, "r") {}

vector<Move> Rook::calcPossibleMoves(Piece* board[8][8]) const {
    return getHorizontalAndVerticalMovements(board);
}
