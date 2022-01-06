#include "Knight.h"

// class pawn, implements the methods from piece (abstract ? )
class Knight : public Piece {
    public:
    Knight(Team team, std::pair<int, int> pos); //constructor

    void calcPossibleMoves(Piece* board[8][8]); 

};