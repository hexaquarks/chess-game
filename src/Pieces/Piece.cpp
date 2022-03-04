#include "../../include/Pieces/Piece.hpp"
#include "../../include/GameThread.hpp"
#include <iostream>

Piece::Piece(Team team, int x, int y, PieceType type, string pieceType):
    m_team(team), m_xPos(x), m_yPos(y), m_type(type) {
    m_filename = String(pieceType + getColorCode() + fileExt);
}

vector<Move> Piece::getHorizontalAndVerticalMovements(Piece* board[8][8]) const {
    vector<Move> moves;
    int xPos = getX(), yPos = getY();

    // Vertical up movement check
    for (int i = xPos-1; i >= 0; --i) {
        if (board[i][yPos] == nullptr) {
            moves.push_back(Move(make_pair(i, yPos), make_pair(xPos, yPos), board[xPos][yPos], MoveType::NORMAL));
        } else if (board[i][yPos]->getTeam() != getTeam()) {
            moves.push_back(Move(make_pair(i, yPos), make_pair(xPos, yPos), board[xPos][yPos], MoveType::CAPTURE, board[i][yPos]));
            break;
        } else {
            break;
        }
    }

    // Vertical down movement check
    for (int i = xPos+1; i < 8; ++i) {
        if (board[i][yPos] == nullptr) {
            moves.push_back(Move(make_pair(i, yPos), make_pair(xPos, yPos), board[xPos][yPos], MoveType::NORMAL));
        } else if (board[i][yPos]->getTeam() != getTeam()) {
            moves.push_back(Move(make_pair(i, yPos), make_pair(xPos, yPos), board[xPos][yPos], MoveType::CAPTURE, board[i][yPos]));
            break;
        } else {
            break;
        }
    }

    // Horizontal left movement check
    for (int j = yPos-1; j >= 0; --j) {
        if (board[xPos][j] == nullptr) {
            moves.push_back(Move(make_pair(xPos, j), make_pair(xPos, yPos), board[xPos][yPos], MoveType::NORMAL));
        } else if (board[xPos][j]->getTeam() != getTeam()) {
            moves.push_back(Move(make_pair(xPos, j), make_pair(xPos, yPos), board[xPos][yPos], MoveType::CAPTURE, board[xPos][j]));
            break;
        } else {
            break;
        }
    }

    // Horizontal right movement check
    for (int j = yPos+1; j < 8; ++j) {
        if (board[xPos][j] == nullptr) {
            moves.push_back(Move(make_pair(xPos, j), make_pair(xPos, yPos), board[xPos][yPos], MoveType::NORMAL));
        } else if (board[xPos][j]->getTeam() != getTeam()) {
            moves.push_back(Move(make_pair(xPos, j), make_pair(xPos, yPos), board[xPos][yPos], MoveType::CAPTURE, board[xPos][j]));
            break;
        } else {
            break;
        }
    }

    return moves;
}

vector<Move> Piece::getDiagonalMovements(Piece* board[8][8]) const {
    vector<Move> moves;
    int xPos = getX(), yPos = getY();

    // Up left diagonal
    int i = xPos-1, j = yPos-1;
    while (i >= 0 && j >= 0) {
        if (board[i][j] == nullptr) {
            moves.push_back(Move(make_pair(i, j), make_pair(xPos, yPos), board[xPos][yPos], MoveType::NORMAL));
            --i; --j;
        } else if (board[i][j]->getTeam() != getTeam()) {
            moves.push_back(Move(make_pair(i, j), make_pair(xPos, yPos), board[xPos][yPos], MoveType::CAPTURE, board[i][j]));
            break;
        } else {
            break;
        }
    }

    // Up right diagonal
    i = xPos-1; j = yPos+1;
    while (i >= 0 && j < 8) {
        if (board[i][j] == nullptr) {
            moves.push_back(Move(make_pair(i, j), make_pair(xPos, yPos), board[xPos][yPos], MoveType::NORMAL));
            --i; ++j;
        } else if (board[i][j]->getTeam() != getTeam()) {
            moves.push_back(Move(make_pair(i, j), make_pair(xPos, yPos), board[xPos][yPos], MoveType::CAPTURE, board[i][j]));
            break;
        } else {
            break;
        }
    }

    // Down left diagonal
    i = xPos+1; j = yPos-1;
    while (i < 8 && j >= 0) {
        if (board[i][j] == nullptr) {
            moves.push_back(Move(make_pair(i, j), make_pair(xPos, yPos), board[xPos][yPos], MoveType::NORMAL));
            ++i; --j;
        } else if (board[i][j]->getTeam() != getTeam()) {
            moves.push_back(Move(make_pair(i, j), make_pair(xPos, yPos), board[xPos][yPos], MoveType::CAPTURE, board[i][j]));
            break;
        } else {
            break;
        }
    }

    // Down right diagonal
    i = xPos+1; j = yPos+1;
    while (i < 8 && j < 8) {
        if (board[i][j] == nullptr) {
            moves.push_back(Move(make_pair(i, j), make_pair(xPos, yPos), board[xPos][yPos], MoveType::NORMAL));
            ++i; ++j;
        } else if (board[i][j]->getTeam() != getTeam()) {
            moves.push_back(Move(make_pair(i, j), make_pair(xPos, yPos), board[xPos][yPos], MoveType::CAPTURE, board[i][j]));
            break;
        } else {
            break;
        }
    }

    return moves;
}
