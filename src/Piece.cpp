#include "../include/Piece.hpp"
#include "../include/GameThread.hpp"
#include <iostream>

Piece::Piece(Team team, int x, int y, PieceType type, string pieceType):
    m_team(team), m_xPos(x), m_yPos(y), m_type(type) {
    m_filename = GameThread::getIconPath(pieceType + getColorCode() + fileExt);
}

moveTypes Piece::getHorizontalAndVerticalMovements(Piece* board[8][8]) const {
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
}

moveTypes Piece::getDiagonalMovements(Piece* board[8][8]) const {
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
