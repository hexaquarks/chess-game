#include "Pawn.h"

#include <vector>

Pawn::Pawn(Team team, int xPos, int yPos)
    : Piece(team, xPos, yPos, PieceType::PAWN, "p") {}

void Pawn::calcPossibleMoves(Piece* board[8][8]) const {
    vector<tuple<pair<int, int> , MoveType>> moves;
    int x = Piece::xPos;
    int y = Piece::yPos;

    // can only move one square forward
    if (y + 1 == 7) {
        // pawn is queening

    } else if (y + 1 > 1 && y + 1 <= 6) {
        // normal movement
        if(board[x][y+1] == nullptr) 
                moves.push_back(make_tuple(make_pair(xPos, yPos+1), MoveType::NORMAL));

        if(!Pawn::hasMoved){
            if(board[x][y+2] == nullptr) 
                moves.push_back(make_tuple(make_pair(xPos, yPos+2), MoveType::NORMAL));
        } 
    }
    
    // en passant
    

    // capture
}
