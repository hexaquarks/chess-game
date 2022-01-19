#pragma once
#include "Piece.hpp"

typedef tuple<pair<int, int>, pair<int, int>> moveFullCoords;

class Move {

    moveFullCoords m_secondPieceCoords; // the last move 
    Piece* m_selectedPiece; // the piece that is being selected
    Piece* m_capturedPiece; // the captured piece, the moved rook in castling, or taken pawn in en passant
    MoveType m_MoveType; 

    public:
    Move(int,int,int,int, Piece*, MoveType); // Constructor for NORMAL, INIT SPECIAL and CASTLE
    Move(int,int,int,int, Piece*, Piece*, MoveType); // Constructor for CAPTURE, EN PASSANT
    int m_xTarget; int m_yTarget; // the destination square of the piece that is being moved
    int m_xInit; int m_yInit; // the initial square of the piece moved
    MoveType getMoveType() { return m_MoveType; };
    pair<int,int> getOriginalSquare() { return make_pair(m_xInit, m_yInit); };
    Piece* getSelectedPiece() { return m_selectedPiece; };
    Piece* getCapturedPiece() { return m_capturedPiece; };
    

    moveFullCoords getMoveCoords() const;

};