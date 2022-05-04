#include "../../include/Pieces/Piece.hpp"
#include "../../include/GameThread.hpp"
#include <iostream>

Piece::Piece(Team team_, int x_, int y_, PieceType type_, string pieceType_):
    m_team(team_), m_xPos(x_), m_yPos(y_), m_type(type_) {
    m_filename = String(pieceType_ + getColorCode() + fileExt);
}

vector<Move> Piece::getHorizontalAndVerticalMovements(Piece* pBoard_[8][8]) const {
    vector<Move> moves;
    int xPos = getX();
    int yPos = getY();

    // Vertical up movement check
    for (int i = xPos-1; i >= 0; --i) {
        if (pBoard_[i][yPos] == nullptr) {
            moves.push_back(Move(make_pair(i, yPos), make_pair(xPos, yPos), pBoard_[xPos][yPos], MoveType::NORMAL));
        } else if (pBoard_[i][yPos]->getTeam() != getTeam()) {
            moves.push_back(Move(make_pair(i, yPos), make_pair(xPos, yPos), pBoard_[xPos][yPos], MoveType::CAPTURE, pBoard_[i][yPos]));
            break;
        } else {
            break;
        }
    }

    // Vertical down movement check
    for (int i = xPos+1; i < 8; ++i) {
        if (pBoard_[i][yPos] == nullptr) {
            moves.push_back(Move(make_pair(i, yPos), make_pair(xPos, yPos), pBoard_[xPos][yPos], MoveType::NORMAL));
        } else if (pBoard_[i][yPos]->getTeam() != getTeam()) {
            moves.push_back(Move(make_pair(i, yPos), make_pair(xPos, yPos), pBoard_[xPos][yPos], MoveType::CAPTURE, pBoard_[i][yPos]));
            break;
        } else {
            break;
        }
    }

    // Horizontal left movement check
    for (int j = yPos-1; j >= 0; --j) {
        if (pBoard_[xPos][j] == nullptr) {
            moves.push_back(Move(make_pair(xPos, j), make_pair(xPos, yPos), pBoard_[xPos][yPos], MoveType::NORMAL));
        } else if (pBoard_[xPos][j]->getTeam() != getTeam()) {
            moves.push_back(Move(make_pair(xPos, j), make_pair(xPos, yPos), pBoard_[xPos][yPos], MoveType::CAPTURE, pBoard_[xPos][j]));
            break;
        } else {
            break;
        }
    }

    // Horizontal right movement check
    for (int j = yPos+1; j < 8; ++j) {
        if (pBoard_[xPos][j] == nullptr) {
            moves.push_back(Move(make_pair(xPos, j), make_pair(xPos, yPos), pBoard_[xPos][yPos], MoveType::NORMAL));
        } else if (pBoard_[xPos][j]->getTeam() != getTeam()) {
            moves.push_back(Move(make_pair(xPos, j), make_pair(xPos, yPos), pBoard_[xPos][yPos], MoveType::CAPTURE, pBoard_[xPos][j]));
            break;
        } else {
            break;
        }
    }

    return moves;
}

vector<Move> Piece::getDiagonalMovements(Piece* pBoard_[8][8]) const {
    vector<Move> moves;
    int xPos = getX();
    int yPos = getY();

    // Up left diagonal
    int i = xPos-1, j = yPos-1;
    while (i >= 0 && j >= 0) {
        if (pBoard_[i][j] == nullptr) {
            moves.push_back(Move(make_pair(i, j), make_pair(xPos, yPos), pBoard_[xPos][yPos], MoveType::NORMAL));
            --i; --j;
        } else if (pBoard_[i][j]->getTeam() != getTeam()) {
            moves.push_back(Move(make_pair(i, j), make_pair(xPos, yPos), pBoard_[xPos][yPos], MoveType::CAPTURE, pBoard_[i][j]));
            break;
        } else {
            break;
        }
    }

    // Up right diagonal
    i = xPos-1; j = yPos+1;
    while (i >= 0 && j < 8) {
        if (pBoard_[i][j] == nullptr) {
            moves.push_back(Move(make_pair(i, j), make_pair(xPos, yPos), pBoard_[xPos][yPos], MoveType::NORMAL));
            --i; ++j;
        } else if (pBoard_[i][j]->getTeam() != getTeam()) {
            moves.push_back(Move(make_pair(i, j), make_pair(xPos, yPos), pBoard_[xPos][yPos], MoveType::CAPTURE, pBoard_[i][j]));
            break;
        } else {
            break;
        }
    }

    // Down left diagonal
    i = xPos+1; j = yPos-1;
    while (i < 8 && j >= 0) {
        if (pBoard_[i][j] == nullptr) {
            moves.push_back(Move(make_pair(i, j), make_pair(xPos, yPos), pBoard_[xPos][yPos], MoveType::NORMAL));
            ++i; --j;
        } else if (pBoard_[i][j]->getTeam() != getTeam()) {
            moves.push_back(Move(make_pair(i, j), make_pair(xPos, yPos), pBoard_[xPos][yPos], MoveType::CAPTURE, pBoard_[i][j]));
            break;
        } else {
            break;
        }
    }

    // Down right diagonal
    i = xPos+1; j = yPos+1;
    while (i < 8 && j < 8) {
        if (pBoard_[i][j] == nullptr) {
            moves.push_back(Move(make_pair(i, j), make_pair(xPos, yPos), pBoard_[xPos][yPos], MoveType::NORMAL));
            ++i; ++j;
        } else if (pBoard_[i][j]->getTeam() != getTeam()) {
            moves.push_back(Move(make_pair(i, j), make_pair(xPos, yPos), pBoard_[xPos][yPos], MoveType::CAPTURE, pBoard_[i][j]));
            break;
        } else {
            break;
        }
    }

    return moves;
}
