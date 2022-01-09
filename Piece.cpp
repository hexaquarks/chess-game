#include "Piece.h"
#include <iostream>

Piece::Piece(Team team, int x, int y, PieceType type, string fileName): team(team), xPos(x), yPos(y), type(type) {
    filename = string("./assets/") + fileName + string(team == Team::BLACK? "b": "w") + string(".png");
}


