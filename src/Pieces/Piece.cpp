#include "../../include/Pieces/Piece.hpp"
#include "../../include/GameThread.hpp"

#include <iostream>

Piece::Piece(Team team_, int x_, int y_, PieceType type_, const string& pieceType_):
    m_team(team_), m_xPos(x_), m_yPos(y_), m_type(type_)
{
    m_filename = pieceType_ + getColorCode() + fileExt;
}

void Piece::addHorizontalAndVerticalMovements(Board& board_, vector<Move>& moves) const
{
    int xPos = getX();
    int yPos = getY();
    shared_ptr<Piece> piece = board_.getBoardTile(yPos, xPos);

    // Vertical up movement check
    for (int i = xPos-1; i >= 0; --i)
    {
        if (!board_.getBoardTile(yPos, i))
        {
            moves.push_back(Move({i, yPos}, {xPos, yPos}, piece, MoveType::NORMAL));
        }
        else if (board_.getBoardTile(yPos, i)->getTeam() != getTeam())
        {
            shared_ptr<Piece> p = board_.getBoardTile(yPos, i);
            moves.push_back(Move({i, yPos}, {xPos, yPos}, piece, MoveType::CAPTURE, p));
            break;
        }
        else
        {
            break;
        }
    }

    // Vertical down movement check
    for (int i = xPos+1; i < 8; ++i)
    {
        if (!board_.getBoardTile(yPos, i))
        {
            moves.push_back(Move({i, yPos}, {xPos, yPos}, piece, MoveType::NORMAL));
        }
        else if (board_.getBoardTile(yPos, i)->getTeam() != getTeam())
        {
            shared_ptr<Piece> p = board_.getBoardTile(yPos, i);
            moves.push_back(Move({i, yPos}, {xPos, yPos}, piece, MoveType::CAPTURE, p));
            break;
        }
        else
        {
            break;
        }
    }

    // Horizontal left movement check
    for (int j = yPos-1; j >= 0; --j)
    {
        if (!board_.getBoardTile(j, xPos))
        {
            moves.push_back(Move({xPos, j}, {xPos, yPos}, piece, MoveType::NORMAL));
        }
        else if (board_.getBoardTile(j, xPos)->getTeam() != getTeam())
        {
            shared_ptr<Piece> p = board_.getBoardTile(j, xPos);
            moves.push_back(Move({xPos, j}, {xPos, yPos}, piece, MoveType::CAPTURE, p));
            break;
        }
        else
        {
            break;
        }
    }

    // Horizontal right movement check
    for (int j = yPos+1; j < 8; ++j)
    {
        if (!board_.getBoardTile(j, xPos))
        {
            moves.push_back(Move({xPos, j}, {xPos, yPos}, piece, MoveType::NORMAL));
        }
        else if (board_.getBoardTile(j, xPos)->getTeam() != getTeam())
        {
            shared_ptr<Piece> p = board_.getBoardTile(j, xPos);
            moves.push_back(Move({xPos, j}, {xPos, yPos}, piece, MoveType::CAPTURE, p));
            break;
        }
        else
        {
            break;
        }
    }
}

void Piece::addDiagonalMovements(Board& board_, vector<Move>& moves) const
{
    int xPos = getX();
    int yPos = getY();
    shared_ptr<Piece> piece = board_.getBoardTile(yPos, xPos);

    // Up left diagonal
    int i = xPos-1, j = yPos-1;
    while (i >= 0 && j >= 0)
    {
        shared_ptr<Piece> p = board_.getBoardTile(j, i);
        if (!p)
        {
            moves.push_back(Move({i, j}, {xPos, yPos}, piece, MoveType::NORMAL));
            --i; --j;
        }
        else if (p->getTeam() != getTeam())
        {
            moves.push_back(Move({i, j}, {xPos, yPos}, piece, MoveType::CAPTURE, p));
            break;
        }
        else
        {
            break;
        }
    }

    // Up right diagonal
    i = xPos-1; j = yPos+1;
    while (i >= 0 && j < 8)
    {
        shared_ptr<Piece> p = board_.getBoardTile(j, i);
        if (!p)
        {
            moves.push_back(Move({i, j}, {xPos, yPos}, piece, MoveType::NORMAL));
            --i; ++j;
        }
        else if (p->getTeam() != getTeam())
        {
            moves.push_back(Move({i, j}, {xPos, yPos}, piece, MoveType::CAPTURE, p));
            break;
        }
        else
        {
            break;
        }
    }

    // Down left diagonal
    i = xPos+1; j = yPos-1;
    while (i < 8 && j >= 0)
    {
        shared_ptr<Piece> p = board_.getBoardTile(j, i);
        if (!p)
        {
            moves.push_back(Move({i, j}, {xPos, yPos}, piece, MoveType::NORMAL));
            ++i; --j;
        }
        else if (p->getTeam() != getTeam())
        {
            moves.push_back(Move({i, j}, {xPos, yPos}, piece, MoveType::CAPTURE, p));
            break;
        }
        else
        {
            break;
        }
    }

    // Down right diagonal
    i = xPos+1; j = yPos+1;
    while (i < 8 && j < 8)
    {
        shared_ptr<Piece> p = board_.getBoardTile(j, i);
        if (!p)
        {
            moves.push_back(Move({i, j}, {xPos, yPos}, piece, MoveType::NORMAL));
            ++i; ++j;
        }
        else if (p->getTeam() != getTeam())
        {
            moves.push_back(Move({i, j}, {xPos, yPos}, piece, MoveType::CAPTURE, p));
            break;
        }
        else
        {
            break;
        }
    }
}

std::ostream& operator<<(std::ostream& os, const PieceType& pieceType) {
    switch(pieceType) {
        case PieceType::PAWN:
            os << "Pawn";
            break;
        case PieceType::KNIGHT:
            os << "Knight";
            break;
        case PieceType::BISHOP:
            os << "Bishop";
            break;
        case PieceType::ROOK:
            os << "Rook";
            break;
        case PieceType::QUEEN:
            os << "Queen";
            break;
        case PieceType::KING:
            os << "King";
            break;
        default:
            os << "Unknown";
            break;
    }
    return os;
}
