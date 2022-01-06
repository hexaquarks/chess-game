#include "Bishop.h"
#include <iostream>

Bishop::Bishop(Team team, std::pair<int,int> pos) : Piece(team, pos, BISHOP) {
    std::string filename;

    filename = team == BLACK ? "./assets/bb.png" : "./assets/bw.png";
    
};