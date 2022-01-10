#include "Pawn.h"
#include <vector>

Pawn::Pawn(Team team, int xPos, int yPos): Piece(team, xPos, yPos, PieceType::PAWN, "p") {}

moveTypes Pawn::calcPossibleMoves(Piece* board[8][8]) const {
    moveTypes moves;
    int dir = (getTeam() == Team::WHITE)? -1: 1;
    int xPos = getX(), yPos = getY();

    // Taking piece on the right
    if (xPos+1 < 8 && (yPos+dir < 8 && yPos+dir >= 0))
        if (board[xPos+1][yPos+dir] != nullptr && board[xPos+1][yPos+dir]->getTeam() != getTeam())
            moves.push_back(make_tuple(make_pair(xPos+1, yPos+dir), MoveType::CAPTURE));

    // Taking piece on the left
    if (xPos-1 >= 0 && (yPos+dir < 8 && yPos+dir >= 0))
        if (board[xPos-1][yPos+dir] != nullptr && board[xPos-1][yPos+dir]->getTeam() != getTeam())
            moves.push_back(make_tuple(make_pair(xPos-1, yPos+dir), MoveType::CAPTURE));

    // Forward move
    if ((xPos+dir == 0 || xPos+dir == 7) && board[xPos+dir][yPos] == nullptr)
        moves.push_back(make_tuple(make_pair(xPos+dir, yPos), MoveType::NEWPIECE));
    else if (board[xPos+dir][yPos] == nullptr) 
        moves.push_back(make_tuple(make_pair(xPos+dir, yPos), MoveType::NORMAL));

    // Double square initial move
    if ((xPos == 1 || xPos == 6) && !hasMoved() && board[xPos+2*dir][yPos] == nullptr)
        moves.push_back(make_tuple(make_pair(xPos+2*dir, yPos), MoveType::INIT_SPECIAL));

    // En passant &TODO
    return moves;
};
