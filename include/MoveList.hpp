#pragma once
#include <list>
#include <SFML/Graphics.hpp>
#include "Board.hpp"
#include "GameThread.hpp"
#include "Move.hpp"
using namespace sf;

class MoveList {
    list<Move> m_moves;
    list<Move>::iterator m_moveIterator = m_moves.begin();
    Board& game;

    void applyMove(MoveType, int, int, int, int, Piece*, Piece*, bool);
    void applyMove();
    void undoMove();

    public:
    MoveList(Board&);
    void highlightLastMove(RenderWindow&) const;
    void reset() { m_moves.clear(); m_moveIterator = m_moves.begin(); };
    void goToPreviousMove();
    void goToNextMove();
    void addMove(MoveType, int, int, int, int, Piece*, Piece*);
    bool hasMovesBefore() const { return m_moveIterator != m_moves.end(); }
    bool hasMovesAfter() const { return m_moveIterator != m_moves.begin(); }
};
