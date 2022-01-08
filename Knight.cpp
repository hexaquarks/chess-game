#include "Knight.h"

Knight::Knight(Team team, int x, int y): Piece(team, x, y, PieceType::KNIGHT, "n") {}

void Knight::calcPossibleMoves(Piece* board[8][8]) const {}
