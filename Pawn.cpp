#include "Pawn.h"
#include <iostream>

Pawn::Pawn(Team team, std::pair<int,int> pos) : Piece(team, pos, PAWN) {
    filename = team == BLACK ? "./assets/pb.png" : "./assets/pw.png";

};