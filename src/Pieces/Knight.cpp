#include "../../include/Pieces/Knight.hpp"
#include <vector>

Knight::Knight(Team team, int x, int y): Piece(team, x, y, PieceType::KNIGHT, "n") {}

vector<Move> Knight::calcPossibleMoves(Piece* board_[8][8]) const {
    vector<Move> moves;
    int xPos = getX(); 
    int yPos = getY();

    // The possible moves
    int X[8] = {2, 1, -1, -2, -2, -1, 1, 2};
    int Y[8] = {1, 2, 2, 1, -1, -2, -2, -1};

    for (int i = 0; i < 8; ++i) {
        int x = xPos + X[i];
        int y = yPos + Y[i];

        if (x >= 0 && y >= 0 && x < 8 && y < 8) {
            if (board_[x][y] == nullptr)
                moves.push_back(Move(make_pair(x, y), make_pair(xPos, yPos), board_[xPos][yPos], MoveType::NORMAL));
            else if (board_[x][y]->getTeam() != getTeam())
                moves.push_back(Move(make_pair(x, y), make_pair(xPos, yPos), board_[xPos][yPos], MoveType::CAPTURE));
        }
    }

    return moves;
}
