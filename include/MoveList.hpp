#pragma once
#include <list>
#include <SFML/Graphics.hpp>
#include "Board.hpp"
#include "GameThread.hpp"
#include "Move.hpp"
using namespace sf;

class MoveList {
    list<Move> moves;
    list<Move>::iterator moveIterator = moves.begin();
    Board& game;

    void applyMove(MoveType, int, int, int, int, Piece*, Piece*);
    void applyMove();
    void undoMove();

    public:
    MoveList(Board&);
    void highlightLastMove(RenderWindow&) const;
    void goToPreviousMove();
    void goToNextMove();
    void addMove(MoveType, int, int, int, int, Piece*, Piece*);
    bool hasMovesBefore() const { return moveIterator != moves.end(); }
    bool hasMovesAfter() const { return moveIterator != moves.begin(); }
};
