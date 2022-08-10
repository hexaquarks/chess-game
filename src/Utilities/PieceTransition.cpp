#include "../../include/Pieces/Piece.hpp"
#include "../../include/Utilities/PieceTransition.hpp"
#include "../../include/GameThread.hpp"
#include <math.h>

// Divide by 6 so the increment is in base 10, that is 60 / 10
// Avoiding any piece to jump a square in a game tick
constexpr uint16_t divisor = g_FPS / 6;


void PieceTransition::setIncrement()
{
    if (!m_piece) return;
    m_increment.first = (m_destination.first - m_currPos.first) / divisor;
    m_increment.second = (m_destination.second - m_currPos.second) / divisor;
}

void PieceTransition::setSecondIncrement() {
    if (!m_second_piece) return;
    m_second_increment.first = (m_second_destination.first - m_second_currPos.first) / divisor;
    m_second_increment.second = (m_second_destination.second - m_second_currPos.second) / divisor;
}

bool PieceTransition::pieceIsInBounds() const
{
    return xArrived() && yArrived() && xSecondArrived() && ySecondArrived();
}

void PieceTransition::move(Board& game)
{
    if (!m_piece) return;

    // Move the piece only if piece is not in destinantion bounds yet.
    if (!xArrived()) m_currPos.first += m_increment.first;
    if (!yArrived()) m_currPos.second += m_increment.second;
    if (!xSecondArrived()) m_second_currPos.first += m_second_increment.first;
    if (!ySecondArrived()) m_second_currPos.second += m_second_increment.second;

    if (pieceIsInBounds())
    {
        setHasArrived(game);
        return;
    }

    updateDistToDest();
}

void PieceTransition::setHasArrived(Board& board_)
{
    // Piece has arrived at destination
    m_hasArrived = true;
    m_captured.reset();
    distance_to_dest = 0.0;
    m_isTransitioning = false;
    m_increment = {0, 0};

    m_second_hasArrived = true;
    m_second_isTransitioning = false;
    m_second_increment = {0, 0};
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

void PieceTransition::setSecondCurrPos(coor2d&& pos)
{
    m_second_currPos = pos;
}

void PieceTransition::updateDistToDest()
{
    int dx = m_destination.first - m_currPos.first;
    int dy = m_destination.second - m_currPos.second;
    distance_to_dest = sqrt(dx*dx + dy*dy);
}
