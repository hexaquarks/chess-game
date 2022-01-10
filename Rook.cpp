#include "Rook.h"
#include <vector>

Rook::Rook(Team team, int x, int y): Piece(team, x, y, PieceType::ROOK, "r") {}

moveTypes Rook::calcPossibleMoves(Piece* board[8][8]) const {
    moveTypes moves;
    int xPos = getX(), yPos = getY();

    // Vertical up movement check
    for (int i = xPos-1; i >= 0; --i) {
        if (board[i][yPos] == nullptr) {
            moves.push_back(make_tuple(make_pair(i, yPos), MoveType::NORMAL));
        } else if (board[i][yPos]->getTeam() != getTeam()) {
            moves.push_back(make_tuple(make_pair(i, yPos), MoveType::CAPTURE));
            break;
        } else {
            break;
        }
    }

    // Vertical down movement check
    for (int i = xPos+1; i < 8; ++i) {
        if (board[i][yPos] == nullptr) {
            moves.push_back(make_tuple(make_pair(i, yPos), MoveType::NORMAL));
        } else if (board[i][yPos]->getTeam() != getTeam()) {
            moves.push_back(make_tuple(make_pair(i, yPos), MoveType::CAPTURE));
            break;
        } else {
            break;
        }
    }

    // Horizontal left movement check
    for (int j = yPos-1; j >= 0; --j) {
        if (board[xPos][j] == nullptr) {
            moves.push_back(make_tuple(make_pair(xPos, j), MoveType::NORMAL));
        } else if (board[xPos][j]->getTeam() != getTeam()) {
            moves.push_back(make_tuple(make_pair(xPos, j), MoveType::CAPTURE));
            break;
        } else {
            break;
        }
    }

    // Horizontal right movement check
    for (int j = yPos+1; j < 8; ++j) {
        if (board[xPos][j] == nullptr) {
            moves.push_back(make_tuple(make_pair(xPos, j), MoveType::NORMAL));
        } else if (board[xPos][j]->getTeam() != getTeam()) {
            moves.push_back(make_tuple(make_pair(xPos, j), MoveType::CAPTURE));
            break;
        } else {
            break;
        }
    }

    return moves;
};
