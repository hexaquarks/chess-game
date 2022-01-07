#include "Knight.h"

Knight::Knight(Team team, std::pair<int,int> pos) : Piece(team, pos, KNIGHT) {
    Piece::_filename = team == BLACK ? "./assets/nb.png" : "./assets/nw.png";
    
};