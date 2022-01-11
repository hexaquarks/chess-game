#include "Pawn.h"
#include <vector>

Pawn::Pawn(Team team, int xPos, int yPos): Piece(team, xPos, yPos, PieceType::PAWN, "p") {}

moveTypes Pawn::calcPossibleMoves(Piece* board[8][8]) const {
    moveTypes moves;
    int dir = (getTeam() == Team::WHITE)? -1: 1;
    int xPos = getX(), yPos = getY();

    generateCaptureMoves(moves, board, dir);
    generateForwardMoves(moves, board, dir);
    generateEnPassantMoves(moves, board);

    return moves;
};

void Pawn::generateCaptureMoves(moveTypes &moves, Piece* board[8][8], int dir) const {
    int xPos =getX(); int yPos = getY();
     // Taking piece on the right
    if (yPos+1 < 8 && (xPos+dir < 8 && xPos+dir >= 0))
        if (board[xPos+dir][yPos+1] != nullptr && board[xPos+dir][yPos+1]->getTeam() != getTeam())
            moves.push_back(make_tuple(make_pair(xPos+dir, yPos+1), MoveType::CAPTURE));

    // Taking piece on the left
    if (yPos-1 >= 0 && (xPos+dir < 8 && xPos+dir >= 0))
        if (board[xPos+dir][yPos-1] != nullptr && board[xPos+dir][yPos-1]->getTeam() != getTeam())
            moves.push_back(make_tuple(make_pair(xPos+dir, yPos-1), MoveType::CAPTURE));
}

void Pawn::generateForwardMoves(moveTypes &moves, Piece* board[8][8], int dir) const {
    int xPos =getX(); int yPos = getY();

    // Forward move
    if ((xPos+dir == 0 || xPos+dir == 7) && board[xPos+dir][yPos] == nullptr)
        moves.push_back(make_tuple(make_pair(xPos+dir, yPos), MoveType::NEWPIECE));
    else if (board[xPos+dir][yPos] == nullptr)  {
        moves.push_back(make_tuple(make_pair(xPos+dir, yPos), MoveType::NORMAL));
        // Double square initial move
        if (!hasMoved() && board[xPos+2*dir][yPos] == nullptr)
            moves.push_back(make_tuple(make_pair(xPos+2*dir, yPos), MoveType::INIT_SPECIAL));
    }

}

void Pawn::generateEnPassantMoves(moveTypes &moves, Piece* board[8][8])  const {
    int xPos =getX(); int yPos = getY();


    // forward en passant
    if (xPos == 3){
        if(board[xPos][yPos-1] == getLastPawn() && getTeam() != getLastPawn()->getTeam()) {
            if (yPos > 0) moves.push_back(make_tuple(make_pair(xPos-1, yPos-1), MoveType::ENPASSANT));
        } 
        if(board[xPos][yPos+1] == getLastPawn() && getTeam() != getLastPawn()->getTeam()) {
            if (yPos < 7) moves.push_back(make_tuple(make_pair(xPos-1, yPos+1), MoveType::ENPASSANT));
        }
    }

    // downward en passant
    if (xPos == 4){
        if(board[xPos][yPos-1] == getLastPawn() && getTeam() != getLastPawn()->getTeam()) {
            if (yPos > 0) moves.push_back(make_tuple(make_pair(xPos+1, yPos-1), MoveType::ENPASSANT));
        } 
        if(board[xPos][yPos+1] == getLastPawn() && getTeam() != getLastPawn()->getTeam()) {
            if (yPos < 7) moves.push_back(make_tuple(make_pair(xPos+1, yPos+1), MoveType::ENPASSANT));
        }
    }
}
