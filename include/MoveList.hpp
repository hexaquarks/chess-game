#pragma once
#include <list>
#include <SFML/Graphics.hpp>
#include "Board.hpp"
#include "GameThread.hpp"
#include "Move.hpp"
#include "PieceTransition.hpp"
using namespace sf;


class MoveList {
    list<Move> m_moves;
    list<Move>::iterator m_moveIterator = m_moves.begin();
    PieceTransition& m_transitioningPiece;
    Board& game;

    void applyMove(Move&, bool, bool); // called inside GameThread ?
    void applyMove(bool); 
    void undoMove(bool);

    public:
    MoveList(Board&, PieceTransition&);
    PieceTransition& getTransitioningPiece() { return m_transitioningPiece; }
    void highlightLastMove(RenderWindow&) const;
    void reset() { m_moves.clear(); m_moveIterator = m_moves.begin(); };
    void goToPreviousMove(bool);
    void goToNextMove(bool);
    void goToCurrentMove() {while(hasMovesAfter()) goToNextMove(false); }
    void goToInitialMove() {while(hasMovesBefore()) goToPreviousMove(false); }
    void addMove(Move&);
    bool hasMovesBefore() const { return m_moveIterator != m_moves.end(); }
    bool hasMovesAfter() const { return m_moveIterator != m_moves.begin(); }
};
