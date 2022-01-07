#include "Queen.h"

Queen::Queen(Team team, std::pair<int,int> pos) : Piece(team, pos, QUEEN) {

    Piece::_filename = team == BLACK ? "./assets/qb.png" : "./assets/qw.png";
    
};