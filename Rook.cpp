#include "Rook.h"
Rook::Rook(Team team, std::pair<int,int> pos) : Piece(team, pos, ROOK) {

    Piece::_filename = team == BLACK ? "./assets/rb.png" : "./assets/rw.png";
    
};