#include "Bishop.h"

Bishop::Bishop(Team team, int x, int y): Piece(team, x, y, PieceType::BISHOP, "b") {}

moveTypes Bishop::calcPossibleMoves(Piece* board[8][8]) const {
    moveTypes moves;
    int xPos = getX(), yPos = getY();

    // Up left diagonal
    int i = xPos-1, j = yPos-1;
    while (i >= 0 && j >= 0) {
        if (board[i][j] == nullptr) {
            moves.push_back(make_tuple(make_pair(i, j), MoveType::NORMAL));
            --i; --j;
        } else if (board[i][j]->getTeam() != getTeam()) {
            moves.push_back(make_tuple(make_pair(i, j), MoveType::CAPTURE));
            break;
        } else {
            break;
        }
    }

    // Up right diagonal
    i = xPos-1; j = yPos+1;
    while (i >= 0 && j < 8) {
        if (board[i][j] == nullptr) {
            moves.push_back(make_tuple(make_pair(i, j), MoveType::NORMAL));
            --i; ++j;
        } else if (board[i][j]->getTeam() != getTeam()) {
            moves.push_back(make_tuple(make_pair(i, j), MoveType::CAPTURE));
            break;
        } else {
            break;
        }
    }

    // Down left diagonal
    i = xPos+1; j = yPos-1;
    while (i < 8 && j >= 0) {
        if (board[i][j] == nullptr) {
            moves.push_back(make_tuple(make_pair(i, j), MoveType::NORMAL));
            ++i; --j;
        } else if (board[i][j]->getTeam() != getTeam()) {
            moves.push_back(make_tuple(make_pair(i, j), MoveType::CAPTURE));
            break;
        } else {
            break;
        }
    }

    // Down right diagonal
    i = xPos+1; j = yPos+1;
    while (i < 8 && j < 8) {
        if (board[i][j] == nullptr) {
            moves.push_back(make_tuple(make_pair(i, j), MoveType::NORMAL));
            ++i; ++j;
        } else if (board[i][j]->getTeam() != getTeam()) {
            moves.push_back(make_tuple(make_pair(i, j), MoveType::CAPTURE));
            break;
        } else {
            break;
        }
    }

    return moves;
}
