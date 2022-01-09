#include "King.h"

King::King(Team team, int x, int y): Piece(team, x, y, PieceType::KING, "k") {}

vector<tuple<pair<int, int> , MoveType>> King::calcPossibleMoves(Piece* board[8][8]) const {
    vector<tuple<pair<int, int> , MoveType>> moves;
    return moves;
}
