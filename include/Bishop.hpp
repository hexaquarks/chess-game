#pragma once
#include "Piece.hpp"


// Represents a bishop
struct Bishop: public Piece {
    Bishop(Team, int, int); // Constructor
    moveTypes calcPossibleMoves(Piece*[8][8]) const override; 
};
