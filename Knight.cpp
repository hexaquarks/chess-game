#include "Knight.h"

Knight::Knight(Team team, int x, int y): Piece(team, x, y, PieceType::KNIGHT, "n") {}

vector<tuple<pair<int, int> , MoveType>> Knight::calcPossibleMoves(Piece* board[8][8]) const {}
