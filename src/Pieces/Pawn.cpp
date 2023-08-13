#include "../../include/Pieces/Pawn.hpp"
#include "../../include/Components/Board.hpp"

#include <vector>

// Index-based coordinates constructor
Pawn::Pawn(Team team_, int file_, int rank_):
    Piece(team_, file_, rank_, PieceType::PAWN, "p")
{
}

// Real coordinates constructor
Pawn::Pawn(Team team_, coor2dChar& coords_):
    Piece(team_, coords_.first, coords_.second, PieceType::PAWN, "p")
{
}
std::vector<Move> Pawn::calcPossibleMoves(Board& board_) const
{
    std::vector<Move> moves;
    int dir = (getTeam() == Team::WHITE)? -1 : 1;
    int rank = getRank();
    int file = getFile();

    generateCaptureMoves(moves, board_, dir);
    generateForwardMoves(moves, board_, dir);
    generateEnPassantMoves(moves, board_, dir);

    return moves;
}

void Pawn::generateCaptureMoves(std::vector<Move>& moves_, Board& board_, int dir_) const
{
    int rank = getRank();
    int file = getFile();
    coor2d pawnCoor = {file, rank};
    std::shared_ptr<Piece> pPawnPos = board_.getBoardTile(file, rank);

    // Taking piece on the right
    if (file+1 < 8 && (rank+dir_ < 8 && rank+dir_ >= 0))
        if (board_.getBoardTile(file+1, rank+dir_) && board_.getBoardTile(file+1, rank+dir_)->getTeam() != getTeam())
        {
            if ((rank+dir_ == 0 || rank+dir_ == 7))
                moves_.push_back(Move({file+1, rank+dir_}, pawnCoor, pPawnPos, MoveType::NEWPIECE));
            else
                moves_.push_back(Move({file+1, rank+dir_}, pawnCoor, pPawnPos, MoveType::CAPTURE));
        }

    // Taking piece on the left
    if (file-1 >= 0 && (rank+dir_ < 8 && rank+dir_ >= 0))
        if (board_.getBoardTile(file-1, rank+dir_) && board_.getBoardTile(file-1, rank+dir_)->getTeam() != getTeam())
        {
            if ((rank+dir_ == 0 || rank+dir_ == 7))
                moves_.push_back(Move({file-1, rank+dir_}, pawnCoor, pPawnPos, MoveType::NEWPIECE));
            else
                moves_.push_back(Move({file-1, rank+dir_}, pawnCoor, pPawnPos, MoveType::CAPTURE));
        }
}

void Pawn::generateForwardMoves(std::vector<Move>& moves_, Board& board_, int dir_) const
{
    int rank = getRank();
    int file = getFile();
    coor2d pawnCoor = {file, rank};
    std::shared_ptr<Piece> pPawnPos = board_.getBoardTile(file, rank);
    bool hasNotMoved = (getTeam() == Team::WHITE && rank == 6) || (getTeam() == Team::BLACK && rank == 1);

    // Forward move
    if ((rank+dir_ == 0 || rank+dir_ == 7) && !board_.getBoardTile(file, rank+dir_))
        moves_.push_back(Move({file, rank+dir_}, pawnCoor, pPawnPos, MoveType::NEWPIECE));
    else if (!board_.getBoardTile(file, rank+dir_))
    {
        moves_.push_back(Move({file, rank+dir_}, pawnCoor, pPawnPos, MoveType::NORMAL));
        // Double square initial move
        if (hasNotMoved && !board_.getBoardTile(file, rank+2*dir_))
            moves_.push_back(Move({file, rank+2*dir_}, pawnCoor, pPawnPos, MoveType::INIT_SPECIAL));
    }
}

void Pawn::generateEnPassantMoves(std::vector<Move>& moves_, Board& board_, int dir) const
{
    int rank = getRank();
    int file = getFile();
    coor2d pawnCoor = {file, rank};
    std::shared_ptr<Piece> pPawnPos = board_.getBoardTile(file, rank);

    // Edge case, should not happen
    if (rank+dir < 0 || rank+dir > 7) return;

    // Left En Passant
    if (file > 0)
    {
        std::shared_ptr<Piece> leftPiece = board_.getBoardTile(file-1, rank);
        if (leftPiece && leftPiece->getType() == PieceType::PAWN && leftPiece->getTeam() != getTeam())
        {
            if (getLastMovedPiece() == leftPiece && leftPiece->getLastMove() == MoveType::INIT_SPECIAL)
            {
                moves_.push_back(Move({file-1, rank+dir}, pawnCoor, pPawnPos, MoveType::ENPASSANT, leftPiece));
            }
        }
    }

    // Right En Passant
    if (file < 7)
    {
        std::shared_ptr<Piece> rightPiece = board_.getBoardTile(file+1, rank);
        if (rightPiece && rightPiece->getType() == PieceType::PAWN && rightPiece->getTeam() != getTeam())
        {
            if (getLastMovedPiece() == rightPiece && rightPiece->getLastMove() == MoveType::INIT_SPECIAL)
            {
                moves_.push_back(Move({file+1, rank+dir}, pawnCoor, pPawnPos, MoveType::ENPASSANT, rightPiece));
            }
        }
    }
}
