#include "King.h"
#include <iostream>

King::King(Team team, std::pair<int,int> pos) : Piece(team, pos, KING) {
    std::string filename;

    filename = team == BLACK ? "./assets/kb.png" : "./assets/kw.png";
    
};