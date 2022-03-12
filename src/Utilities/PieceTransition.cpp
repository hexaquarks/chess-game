#include "../../include/Pieces/Piece.hpp"
#include "../../include/Utilities/PieceTransition.hpp"
#include "../../include/GameThread.hpp"

void PieceTransition::setIncrement() { 
    if (m_piece == nullptr) return; 

    // piece should arrive at destination in one second
    m_increment.first = (m_destination.first - m_currPos.first) / (FPS / 6);
    m_increment.second = (m_destination.second - m_currPos.second) / (FPS / 6);

    // divide by 6 so the  increment is in base 10 , that is 60 / 10
    // avoiding any piece to jump a square in a game tick
}

bool PieceTransition::pieceIsInBounds() {
    if (m_piece == nullptr) return false;

    // stop if arrived in the tile for temporary logical simplicity
    if (m_currPos.first == m_destination.first )  m_xArrived = true;
    if (m_currPos.second == m_destination.second ) m_yArrived = true;

    return m_xArrived && m_yArrived;
}

void PieceTransition::move() { 
    if (m_piece == nullptr) return; 
    cout << m_destination.first << "," << m_destination.second << endl;
    // move the piece only if piece is not in destinantion bounds yet.
    if (!m_xArrived) m_currPos.first += m_increment.first;
    if (!m_yArrived) m_currPos.second += m_increment.second;
}

void PieceTransition::setHasArrived(bool b, Board& board) {
    m_hasArrived = b;
    if (m_hasArrived) {
        // piece has arrived at destination 
        m_isTransitioning = false;
        m_xArrived = false;
        m_yArrived = false;
        m_increment = {0,0};
        board.setBoardTile(m_currPos.first / CELL_SIZE , m_currPos.second / CELL_SIZE, m_piece);
        //  m_piece = nullptr;
    }
}
