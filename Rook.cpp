#include "Rook.h"
#include <iostream>

Rook::Rook(Team team, std::pair<int,int> pos) : Piece(team, pos, ROOK) {
    std::string filename;

    filename = team == BLACK ? "./assets/rb.png" : "./assets/rw.png";
    
};