#include "Queen.h"
#include <vector>

Queen::Queen(Team team, int x, int y): Piece(team, x, y, PieceType::QUEEN, "q") {}

moveTypes Queen::calcPossibleMoves(Piece* board[8][8]) const {
    moveTypes moves;
    return moves;
};
