#include "Queen.h"
#include <iostream>

Queen::Queen(Team team, std::pair<int,int> pos) : Piece(team, pos, QUEEN) {
    std::string filename;

    filename = team == BLACK ? "./assets/qb.png" : "./assets/qw.png";
    
};