#pragma once
#include <SFML/Graphics.hpp>
#include "Board.hpp"
using namespace sf;


class GameThread {    
    static void initializeBoard(RenderWindow&, Board&);
    static void drawCaptureCircles(RenderWindow&, moveTypes&, Board&);
    static void highlightHoveredSquare(RenderWindow&,Board&, moveTypes&, int,int);
    static void drawPieces(RenderWindow&, Board&);
    static void drawDraggedPiece(Piece*, RenderWindow&, int, int);
    static void highlightLastMove(RenderWindow&, list<Move>::iterator&);
    static void removeIllegalMoves(Board&, moveTypes&, Piece*, int,int);
    static void goToPreviousMove(Board&,list<Move>&, list<Move>::iterator&);
    static void goToNextMove(Board&, list<Move>&, list<Move>::iterator&);

    public:
    GameThread() = delete; // Delete constructor
    static void startGame();
};
