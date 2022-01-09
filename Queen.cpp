#include "Queen.h"
#include <vector>

Queen::Queen(Team team, int x, int y): Piece(team, x, y, PieceType::QUEEN, "q") {}

vector<tuple<pair<int, int> , MoveType>> Queen::calcPossibleMoves(Piece* board[8][8]) const {
    vector<tuple<pair<int, int> , MoveType>> moves;
    return moves;
};
