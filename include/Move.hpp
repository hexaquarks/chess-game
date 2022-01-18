#include "Piece.hpp"

typedef tuple<pair<int, int>, pair<int, int>> moveFullCoords;

class Move {

    int m_xTarget; int m_yTarget; // the destination square of the piece that is being moved
    moveFullCoords m_secondPieceCoords; // the last move 
    Piece* m_selectedPiece; // the piece that is being selected
    Piece* m_takenOrMovedPiece; // the captured piece, the moved rook in castling, or taken pawn in en passant
    moveType* m_moveType; 

    public:
    Move(int,int, Piece*, moveType*); // Constructor for NORMAL and INIT SPECIAL
    Move(int,int, Piece*, Piece*, moveType*); // Constructor for CAPTURE
    Move(int,int, Piece*, Piece*, moveType*, moveFullCoords); // Constructor EN PASSANT and CASTLE
    

    moveFullCoords getMoveCoords() const;

};