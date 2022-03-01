#include "../include/Bishop.hpp"

Bishop::Bishop(Team team, int x, int y): Piece(team, x, y, PieceType::BISHOP, "b") {}

vector<Move> Bishop::calcPossibleMoves(Piece* board[8][8]) const {
    return getDiagonalMovements(board);
}
