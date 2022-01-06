#include "Piece.h"

// class pawn, implements the methods from piece (abstract ? )
class Bishop : public Piece {
    public:
    Bishop(Team team, std::pair<int, int> pos); //constructor

    void calcPossibleMoves(Piece* board[8][8]); 

};