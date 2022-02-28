#include "../include/Rook.hpp"
#include <vector>

Rook::Rook(Team team, int x, int y): Piece(team, x, y, PieceType::ROOK, "r") {}

moveTypes Rook::calcPossibleMoves(Piece* board[8][8]) const {
    return getHorizontalAndVerticalMovements(board);
}
