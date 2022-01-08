#include "Rook.h"
#include <vector>

Rook::Rook(Team team, int x, int y): Piece(team, x, y, PieceType::ROOK, "r") {}

vector<tuple<pair<int, int> , MoveType>> Rook::calcPossibleMoves(Piece* board[8][8]) const {
    vector<tuple<pair<int, int> , MoveType>> moves;
    int xPos = Rook::xPos;
    int yPos = Rook::yPos;

    // horizontal movement
    for(int i = 0 ; i < 8; ++i){
        //rook can't move to it's own square
        if(xPos == i || yPos == i) continue;

        // distance between the rook and the target square
        int dx = xPos - i;
        int dy = yPos - i;

        // check if there is already a piece between the rook and the destination square (interference)
        bool horizontalPieceInterference;
        bool verticalPieceInterference;

        // in horizontal direction
        for(int j = 0 ; j < std::abs(dx) ; ++j){
            if(dx >= 0 && board[xPos - j][yPos] != nullptr) {
                // to the left of the rook 
                horizontalPieceInterference = true;
                break;
            }
            if(dx < 0 && board[xPos + j][yPos] != nullptr) {
                // to the right of the rook
                horizontalPieceInterference = true;
                break;
            }
        }

        // in vertical direction
        for(int j = 0 ; j < std::abs(dy) ; ++j){
            if(dx >= 0 && board[xPos][yPos - j] != nullptr) {
                // upwards to the rook 
                verticalPieceInterference = true;
                break;
            }
            if(dx < 0 && board[xPos][yPos + j] != nullptr) {
                // downwards to the rook
                verticalPieceInterference = true;
                break;
            }
        }

        // adding the moves to the list
        if(!horizontalPieceInterference) {
            if(board[i][yPos]->getTeam() != Rook::getTeam()) 
                moves.push_back(make_tuple(make_pair(i,yPos), MoveType::CAPTURE));
            else 
                moves.push_back(make_tuple(make_pair(i,yPos), MoveType::NORMAL));
        }
        if(!verticalPieceInterference) {
            if(board[xPos][i]->getTeam() != Rook::getTeam()) 
                moves.push_back(make_tuple(make_pair(xPos,i), MoveType::CAPTURE));
            else 
                moves.push_back(make_tuple(make_pair(xPos,i), MoveType::NORMAL));
        }


    }
};
