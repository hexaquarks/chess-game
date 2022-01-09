#include "King.h"

King::King(Team team, int x, int y): Piece(team, x, y, PieceType::KING, "k") {}

moveTypes King::calcPossibleMoves(Piece* board[8][8]) const {
    moveTypes moves;
    return moves;
}
