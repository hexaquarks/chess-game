#include "Queen.h"

Queen::Queen(Team team, int x, int y): Piece(team, x, y, PieceType::QUEEN, "q") {}

void Queen::calcPossibleMoves(Piece*s[8][8]) const {}
