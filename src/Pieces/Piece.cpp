#include "../../include/Pieces/Piece.hpp"
#include "../../include/GameThread.hpp"

#include <iostream>

// Index-based coordinates constructor ({0, 0} == {8, 'a'})
Piece::Piece(Team team_, int rank_, int file_, PieceType type_, const string& pieceType_)
: m_team(team_), 
  m_rank(rank_), 
  m_file(file_), 
  m_type(type_)
{
    m_filename = pieceType_ + getColorCode() + fileExt;
}

// real coordinates constructor ({8, 'a'} == {0, 0})
Piece::Piece(Team team_, coor2dChar& coords_, PieceType type_, const string& pieceType_)
: m_team(team_), 
  m_rank(8 - coords_.first), 
  m_file(coords_.second - 'a'), 
  m_type(type_)
{
    m_filename = pieceType_ + getColorCode() + fileExt;
}


void Piece::addHorizontalAndVerticalMovements(Board& board_, vector<Move>& moves) const
{
    int rank = getRank();
    int file = getFile();
    shared_ptr<Piece> piece = board_.getBoardTile(file, rank);

    // Vertical up movement check
    for (int i = rank - 1; i >= 0; --i)
    {
        if (!board_.getBoardTile(file, i))
        {
            moves.push_back(Move({i, file}, {rank, file}, piece, MoveType::NORMAL));
        }
        else if (board_.getBoardTile(file, i)->getTeam() != getTeam())
        {
            shared_ptr<Piece> p = board_.getBoardTile(file, i);
            moves.push_back(Move({i, file}, {rank, file}, piece, MoveType::CAPTURE, p));
            break;
        }
        else break;
    }

    // Vertical down movement check
    for (int i = rank + 1; i < 8; ++i)
    {
        if (!board_.getBoardTile(file, i))
        {
            moves.push_back(Move({i, file}, {rank, file}, piece, MoveType::NORMAL));
        }
        else if (board_.getBoardTile(file, i)->getTeam() != getTeam())
        {
            shared_ptr<Piece> p = board_.getBoardTile(file, i);
            moves.push_back(Move({i, file}, {rank, file}, piece, MoveType::CAPTURE, p));
            break;
        }
        else break;
    }

    // Horizontal left movement check
    for (int j = file - 1; j >= 0; --j)
    {
        if (!board_.getBoardTile(j, rank))
        {
            moves.push_back(Move({rank, j}, {rank, file}, piece, MoveType::NORMAL));
        }
        else if (board_.getBoardTile(j, rank)->getTeam() != getTeam())
        {
            shared_ptr<Piece> p = board_.getBoardTile(j, rank);
            moves.push_back(Move({rank, j}, {rank, file}, piece, MoveType::CAPTURE, p));
            break;
        }
        else break;
    }

    // Horizontal right movement check
    for (int j = file + 1; j < 8; ++j)
    {
        if (!board_.getBoardTile(j, rank))
        {
            moves.push_back(Move({rank, j}, {rank, file}, piece, MoveType::NORMAL));
        }
        else if (board_.getBoardTile(j, rank)->getTeam() != getTeam())
        {
            shared_ptr<Piece> p = board_.getBoardTile(j, rank);
            moves.push_back(Move({rank, j}, {rank, file}, piece, MoveType::CAPTURE, p));
            break;
        }
        else break;
    }
}

void Piece::addDiagonalMovements(Board& board_, vector<Move>& moves_) const
{
    int rank = getRank();
    int file = getFile();
    shared_ptr<Piece> piece = board_.getBoardTile(file, rank);

    // Up left diagonal
    int i = rank - 1;
    int j = file - 1;
    while (i >= 0 && j >= 0)
    {
        shared_ptr<Piece> p = board_.getBoardTile(j, i);
        if (!p)
        {
            moves_.push_back(Move({i, j}, {rank, file}, piece, MoveType::NORMAL));
            --i; --j;
        }
        else if (p->getTeam() != getTeam())
        {
            moves_.push_back(Move({i, j}, {rank, file}, piece, MoveType::CAPTURE, p));
            break;
        }
        else break;
    }

    // Up right diagonal
    i = rank - 1; 
    j = file + 1;
    while (i >= 0 && j < 8)
    {
        shared_ptr<Piece> p = board_.getBoardTile(j, i);
        if (!p)
        {
            moves_.push_back(Move({i, j}, {rank, file}, piece, MoveType::NORMAL));
            --i; ++j;
        }
        else if (p->getTeam() != getTeam())
        {
            moves_.push_back(Move({i, j}, {rank, file}, piece, MoveType::CAPTURE, p));
            break;
        }
        else break;
    }

    // Down left diagonal
    i = rank + 1; 
    j = file - 1;
    while (i < 8 && j >= 0)
    {
        shared_ptr<Piece> p = board_.getBoardTile(j, i);
        if (!p)
        {
            moves_.push_back(Move({i, j}, {rank, file}, piece, MoveType::NORMAL));
            ++i; --j;
        }
        else if (p->getTeam() != getTeam())
        {
            moves_.push_back(Move({i, j}, {rank, file}, piece, MoveType::CAPTURE, p));
            break;
        }
        else break;
    }

    // Down right diagonal
    i = rank + 1; 
    j = file + 1;
    while (i < 8 && j < 8)
    {
        shared_ptr<Piece> p = board_.getBoardTile(j, i);
        if (!p)
        {
            moves_.push_back(Move({i, j}, {rank, file}, piece, MoveType::NORMAL));
            ++i; ++j;
        }
        else if (p->getTeam() != getTeam())
        {
            moves_.push_back(Move({i, j}, {rank, file}, piece, MoveType::CAPTURE, p));
            break;
        }
        else break;
    }
}

std::ostream& operator<<(std::ostream& os_, const PieceType& pieceType_) 
{
    switch (pieceType_) 
    {
        case PieceType::PAWN:
            os_ << "Pawn";
            break;
        case PieceType::KNIGHT:
            os_ << "Knight";
            break;
        case PieceType::BISHOP:
            os_ << "Bishop";
            break;
        case PieceType::ROOK:
            os_ << "Rook";
            break;
        case PieceType::QUEEN:
            os_ << "Queen";
            break;
        case PieceType::KING:
            os_ << "King";
            break;
        default:
            os_ << "Unknown";
            break;
    }
    return os_;
}

std::ostream& operator<<(std::ostream& os_, Team team_) 
{
    switch (team_) 
    {
        case Team::WHITE: os_ << "WHITE"; break;
        case Team::BLACK: os_ << "BLACK"; break;
        default: os_ << "UNIMPLEMENTED TEAM"; break;
    }
    return os_;
}