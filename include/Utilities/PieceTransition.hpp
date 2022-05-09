#pragma once
#include "../Pieces/Piece.hpp"
#include "../Components/Board.hpp"
#include <vector> 

typedef std::pair<int, int> coor2d;
constexpr uint16_t g_FPS = 60;

class PieceTransition 
{
    Piece* m_piece;
    coor2d m_currPos{ 0, 0 };
    coor2d m_destination; 
    coor2d m_increment; 
    bool m_hasArrived = false;
    bool m_isTransitioning = false;
    bool m_xArrived= false;
    bool m_yArrived= false;

    public:
    PieceTransition() { m_piece = nullptr; }; 

    Piece* getPiece() { return m_piece; }
    coor2d getIncrement() { return m_increment; }
    coor2d getDestination() const { return m_destination; }
    coor2d getCurrPos() { return m_currPos; }

    // setters
    void setCurrPos(coor2d& pos) { m_currPos = pos; }
    void setDestination(coor2d& dest) { m_destination = dest; }
    void setTransitioningPiece() { m_piece = nullptr; }
    void setTransitioningPiece(Piece* p) { m_piece = p; }
    void setIsTransitioning(bool b) { m_isTransitioning = b; }
    void setIncrement();
    void setHasArrived(bool b, Board&);

    // getters 
    bool getIsTransitioning() { return m_isTransitioning; }
    bool getHasArrived() { return m_hasArrived; }

    // utilities
    bool pieceIsInBounds();
    void move();
};