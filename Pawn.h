#include "Piece.h"
#include <iostream>

// class pawn, implements the methods from piece (abstract ? )
class Pawn : public Piece {
    public:
    Pawn(Team team, std::pair<int, int> pos); //constructor

    std::string filename;
    
    void calcPossibleMoves(Piece* board[8][8]); 

};