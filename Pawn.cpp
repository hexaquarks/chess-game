#include "Pawn.h"

#include <vector>

Pawn::Pawn(Team team, int xPos, int yPos)
    : Piece(team, xPos, yPos, PieceType::PAWN, "p") {}

vector<tuple<pair<int, int> , MoveType>> Pawn::calcPossibleMoves(Piece* board[8][8]) const {
    vector<tuple<pair<int, int> , MoveType>> moves;
    int x = Piece::xPos;
    int y = Piece::yPos;

    int dir = Pawn::getTeam() == Team::WHITE ? 1 : -1;

    /* Capture
     * Need to do each side explicitly
     * 1. taking rightwards
     */ 
    if(x + 1 <= 7 && (y + dir <=7 && y + dir >= 0)) {
        if(board[x+1][y+dir]->getTeam() != Pawn::getTeam()) {
            moves.push_back(make_tuple(make_pair(xPos+1, yPos+dir), MoveType::CAPTURE));
        }        
    } 

    // 2. taking leftwards
    if (x - 1 >=0 && (y + dir <=7 && y + dir >= 0)) {
        if(board[x-1][y+dir]->getTeam() != Pawn::getTeam()) {
            moves.push_back(make_tuple(make_pair(xPos-1, yPos+dir), MoveType::CAPTURE));
        }        
    }

    // normal one tile move 
    if(y + dir == 0 || y + dir == 7) {
        // pawn hitting the edge of the board (queeing)
        if(board[x][y+dir] == nullptr) 
                moves.push_back(make_tuple(make_pair(xPos, yPos+dir), MoveType::NEWPIECE));

    } else {
        // pawn hitting a square on the board
        if(board[x][y+dir] == nullptr) 
                moves.push_back(make_tuple(make_pair(xPos, yPos+dir), MoveType::NORMAL));
    }

    // double square initial move
    if((y == 1 || y == 6) && !Pawn::hasMoved) {
        if(board[x][y+2*dir] == nullptr) 
                moves.push_back(make_tuple(make_pair(xPos, yPos+2*dir), MoveType::NORMAL));
    }

    // En passant &TODO

    return moves;
};
