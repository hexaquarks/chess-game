#include "King.h"

King::King(Team team, std::pair<int,int> pos) : Piece(team, pos, KING) {
    Piece::_filename = team == BLACK ? "./assets/kb.png" : "./assets/kw.png";
    
};