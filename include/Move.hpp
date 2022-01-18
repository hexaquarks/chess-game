#pragma once
#include "Piece.hpp"

typedef tuple<pair<int, int>, pair<int, int>> moveFullCoords;

class Move {

    int m_xTarget; int m_yTarget; // the destination square of the piece that is being moved
    moveFullCoords m_secondPieceCoords; // the last move 
    Piece* m_selectedPiece; // the piece that is being selected
    Piece* m_takenOrMovedPiece; // the captured piece, the moved rook in castling, or taken pawn in en passant
    MoveType m_MoveType; 

    public:
    Move(int,int, Piece*, MoveType); // Constructor for NORMAL, INIT SPECIAL and CASTLE
    Move(int,int, Piece*, Piece*, MoveType); // Constructor for CAPTURE, EN PASSANT
    MoveType getMoveType() { return m_MoveType; };
    

    moveFullCoords getMoveCoords() const;

};