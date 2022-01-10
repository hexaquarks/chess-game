#include "Knight.h"
#include <vector>

Knight::Knight(Team team, int x, int y): Piece(team, x, y, PieceType::KNIGHT, "n") {}

moveTypes Knight::calcPossibleMoves(Piece* board[8][8]) const {
    moveTypes moves;

    // the possible moves
    int X[8] = {2,1,-1,-2,-2,-1,1,2};
    int Y[8] = {1,2,2,1,-1,-2,-2,-1};

    for (int i = 0; i < 8; ++i) {
        int x = getX() + X[i];
        int y = getY() + Y[i];

        if (x >= 0 && y >= 0 && x < 8 && y < 8) {
            if (board[x][y] == nullptr)
                moves.push_back(make_tuple(make_pair(x, y), MoveType::NORMAL));
            else if (board[x][y]->getTeam() != getTeam())
                moves.push_back(make_tuple(make_pair(x, y), MoveType::CAPTURE));
        }
    }
    
    return moves;
};
