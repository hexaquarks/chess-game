#include "../../include/Pieces/Piece.hpp"
#include "../../include/Utilities/PieceTransition.hpp"
#include "../../include/GameThread.hpp"

void PieceTransition::setIncrement()
{
    if (!m_piece) return;

    // Piece should arrive at destination in one second
    m_increment.first = (m_destination.first - m_currPos.first) / (g_FPS / 6);
    m_increment.second = (m_destination.second - m_currPos.second) / (g_FPS / 6);

    // Divide by 6 so the increment is in base 10, that is 60 / 10
    // Avoiding any piece to jump a square in a game tick
}

bool PieceTransition::pieceIsInBounds()
{
    // Stop if arrived in the tile for temporary logical simplicity
    if (m_currPos.first == m_destination.first) m_xArrived = true;
    if (m_currPos.second == m_destination.second) m_yArrived = true;

    return m_xArrived && m_yArrived;
}

void PieceTransition::move(Board& game)
{
    if (!m_piece) return;

    if (pieceIsInBounds())
    {
        setHasArrived(game);
        return;
    }

    // Move the piece only if piece is not in destinantion bounds yet.
    if (!m_xArrived) m_currPos.first += m_increment.first;
    if (!m_yArrived) m_currPos.second += m_increment.second;
    updateDistToDest();
}

void PieceTransition::setHasArrived(Board& board_)
{
    // Piece has arrived at destination
    m_hasArrived = true;
    m_captured.reset();
    distance_to_dest = 0.0;
    m_isTransitioning = false;
    m_xArrived = false;
    m_yArrived = false;
    m_increment = {0, 0};
}

void PieceTransition::setCapturedPiece(shared_ptr<Piece>& captured, int x, int y)
{
    m_captured = captured;
    if (m_captured) {
        capturedXPos = x;
        capturedYPos = y;
    }
}

void PieceTransition::setCurrPos(coor2d&& pos)
{
    m_currPos = pos;
    updateDistToDest();
    original_dest = distance_to_dest;
}

void PieceTransition::updateDistToDest()
{
    int dx = m_destination.first - m_currPos.first;
    int dy = m_destination.second - m_currPos.second;
    distance_to_dest = sqrt(dx*dx + dy*dy);
}
