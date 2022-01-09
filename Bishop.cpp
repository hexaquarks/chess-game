#include "Bishop.h"

Bishop::Bishop(Team team, int x, int y): Piece(team, x, y, PieceType::BISHOP, "b") {}

vector<tuple<pair<int, int> , MoveType>> Bishop::calcPossibleMoves(Piece* board[8][8]) const {
    vector<tuple<pair<int, int> , MoveType>> moves;
    return moves;
}
