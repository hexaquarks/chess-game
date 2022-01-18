#include "Piece.hpp"

class Move {

    Move* m_lastMove;
    Piece* m_selectedPiece;
    Piece* m_takenOrMovedPiece;
    moveType m_moveType; 

    public:
    Move(Piece*, Piece*, moveType, Move*); // Constructor

};