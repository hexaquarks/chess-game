#include "../../include/Pieces/Pawn.hpp"
#include "../../include/Components/Board.hpp"

#include <vector>

Pawn::Pawn(Team team_, int xPos_, int yPos_):
    Piece(team_, xPos_, yPos_, PieceType::PAWN, "p")
{
}

vector<Move> Pawn::calcPossibleMoves(Board& board_) const
{
    vector<Move> moves;
    int dir = (getTeam() == Team::WHITE)? -1 : 1;
    int xPos = getX();
    int yPos = getY();

    generateCaptureMoves(moves, board_, dir);
    generateForwardMoves(moves, board_, dir);
    generateEnPassantMoves(moves, board_, dir);

    return moves;
}

void Pawn::generateCaptureMoves(vector<Move>& moves_, Board& board_, int dir_) const
{
    int xPos = getX();
    int yPos = getY();
    coor2d pawnCoor = { xPos, yPos };
    shared_ptr<Piece> pPawnPos = board_.getBoardTile(yPos, xPos);

    // Taking piece on the right
    if (yPos+1 < 8 && (xPos+dir_ < 8 && xPos+dir_ >= 0))
        if (board_.getBoardTile(yPos+1, xPos+dir_) && board_.getBoardTile(yPos+1, xPos+dir_)->getTeam() != getTeam())
        {
            if ((xPos+dir_ == 0 || xPos+dir_ == 7))
                moves_.push_back(Move(make_pair(xPos+dir_, yPos+1), pawnCoor, pPawnPos, MoveType::NEWPIECE));
            else
                moves_.push_back(Move(make_pair(xPos+dir_, yPos+1), pawnCoor, pPawnPos, MoveType::CAPTURE));
        }

    // Taking piece on the left
    if (yPos-1 >= 0 && (xPos+dir_ < 8 && xPos+dir_ >= 0))
        if (board_.getBoardTile(yPos-1, xPos+dir_) && board_.getBoardTile(yPos-1, xPos+dir_)->getTeam() != getTeam())
        {
            if ((xPos+dir_ == 0 || xPos+dir_ == 7))
                moves_.push_back(Move(make_pair(xPos+dir_, yPos-1), pawnCoor, pPawnPos, MoveType::NEWPIECE));
            else
                moves_.push_back(Move(make_pair(xPos+dir_, yPos-1), pawnCoor, pPawnPos, MoveType::CAPTURE));
        }
}

void Pawn::generateForwardMoves(vector<Move>& moves_, Board& board_, int dir_) const
{
    int xPos = getX();
    int yPos = getY();
    coor2d pawnCoor = {xPos, yPos};
    shared_ptr<Piece> pPawnPos = board_.getBoardTile(yPos, xPos);
    bool hasNotMoved = (getTeam() == Team::WHITE && xPos == 6) || (getTeam() == Team::BLACK && xPos == 1);

    // Forward move
    if ((xPos+dir_ == 0 || xPos+dir_ == 7) && !board_.getBoardTile(yPos, xPos+dir_))
        moves_.push_back(Move(make_pair(xPos+dir_, yPos), pawnCoor, pPawnPos, MoveType::NEWPIECE));
    else if (!board_.getBoardTile(yPos, xPos+dir_))
    {
        moves_.push_back(Move(make_pair(xPos+dir_, yPos), pawnCoor, pPawnPos, MoveType::NORMAL));
        // Double square initial move
        if (hasNotMoved && !board_.getBoardTile(yPos, xPos+2*dir_))
            moves_.push_back(Move(make_pair(xPos+2*dir_, yPos), pawnCoor, pPawnPos, MoveType::INIT_SPECIAL));
    }
}

void Pawn::generateEnPassantMoves(vector<Move>& moves_, Board& board_, int dir) const
{
    int xPos = getX();
    int yPos = getY();
    coor2d pawnCoor = { xPos, yPos };
    shared_ptr<Piece> pPawnPos = board_.getBoardTile(yPos, xPos);

    // Edge case, should not happen
    if (xPos+dir < 0 || xPos+dir > 7) return;

    // Left En Passant
    if (yPos > 0)
    {
        shared_ptr<Piece> leftPiece = board_.getBoardTile(yPos-1, xPos);
        if (leftPiece && leftPiece->getType() == PieceType::PAWN && leftPiece->getTeam() != getTeam())
        {
            if (getLastMovedPiece() == leftPiece && leftPiece->getLastMove() == MoveType::INIT_SPECIAL)
            {
                moves_.push_back(Move(make_pair(xPos+dir, yPos-1), pawnCoor, pPawnPos, MoveType::ENPASSANT, leftPiece));
            }
        }
    }

    // Right En Passant
    if (yPos < 7)
    {
        shared_ptr<Piece> rightPiece = board_.getBoardTile(yPos+1, xPos);
        if (rightPiece && rightPiece->getType() == PieceType::PAWN && rightPiece->getTeam() != getTeam())
        {
            if (getLastMovedPiece() == rightPiece && rightPiece->getLastMove() == MoveType::INIT_SPECIAL)
            {
                moves_.push_back(Move(make_pair(xPos+dir, yPos+1), pawnCoor, pPawnPos, MoveType::ENPASSANT, rightPiece));
            }
        }
    }
}
