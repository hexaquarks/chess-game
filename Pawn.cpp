#include "Pawn.h"

Pawn::Pawn(Team team, std::pair<int,int> pos) : Piece(team, pos, PAWN) {
    Piece::_filename = team == BLACK ? "./assets/pb.png" : "./assets/pw.png";
    
};