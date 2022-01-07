#include "Piece.h"
#include <iostream>

Piece::Piece(Team team, std::pair<int, int> pos,  PieceType type)
	:_team(team), _pos(pos), _type(type) {
}

Piece::~Piece() {}

