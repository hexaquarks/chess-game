#include "../include/Piece.hpp"
#include "../include/PieceTransition.hpp"
#include "../include/GameThread.hpp"

void PieceTransition::setIncrement() { 
    if(m_piece == nullptr) return; 

    // piece should arrive at destination in one second
    m_increment.first = (m_destination.first - m_currPos.first) / 10;
    m_increment.second = (m_destination.second - m_currPos.second) / 10;
    // cout << "destination is " << m_destination.first << "," << m_destination.second << endl;
    // cout << "curr is " << m_currPos.first << "," << m_currPos.second << endl;
    cout << "increment is " << m_increment.second << "," << m_increment.first << endl;
}

bool PieceTransition::pieceIsInBounds() {
    if(m_piece == nullptr ) return false;

    // check x for 5 % pixel 
    // stop if arrived in the tile for temporary logical simplicity
    if (m_currPos.first * CELL_SIZE == m_destination.first * CELL_SIZE)
        m_xArrived = true;
    if (m_currPos.second * CELL_SIZE == m_destination.second * CELL_SIZE)
        m_yArrived = true;

    return m_xArrived && m_yArrived;
}

void PieceTransition::move() { 
    if(m_piece == nullptr) return; 
    // move the piece only if piece is not in destinantion bounds yet.
    m_currPos.first = m_xArrived ? m_currPos.first : m_currPos.first + m_increment.first;
    m_currPos.second = m_yArrived ? m_currPos.second : m_currPos.second + m_increment.second;
    // cout << "curr is " << m_currPos.first << "," << m_currPos.second << endl;
}

void PieceTransition::setHasArrived(bool b, Board& board) {
     m_hasArrived = b;
     if(m_hasArrived) {
         // piece has arrived at destination 
         m_isTransitioning = false;
         m_xArrived = false;
         m_yArrived = false;
         board.setBoardTile(m_currPos.first / CELL_SIZE , m_currPos.second / CELL_SIZE, m_piece);
         cout << "arrived "<< endl;
        //  m_piece = nullptr;
     }
}