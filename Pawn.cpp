#include "Pawn.h"

Pawn::Pawn(Team team, int xPos, int yPos): Piece(team, xPos, yPos, PieceType::PAWN, "p") {}

void Pawn::calcPossibleMoves(Piece* board[8][8]) const {}
