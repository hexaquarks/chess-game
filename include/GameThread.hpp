#pragma once
#include <SFML/Graphics.hpp>
#include "ChessGame.hpp"
using namespace sf;


class GameThread {    
    static void initializeBoard(RenderWindow&);
    static void drawCaptureCircles(RenderWindow&, moveTypes&, ChessGame&);
    static void highlightHoveredSquare(RenderWindow&, moveTypes&, int,int);
    static void drawPieces(RenderWindow&, ChessGame&);
    static void drawDraggedPiece(Piece*, RenderWindow&, int, int);
    static void highlightLastMove(Piece*, RenderWindow&, int, int);
    static void removeIllegalMoves(ChessGame&, moveTypes&, Piece*, int,int);
    static void goToPreviousMove(ChessGame&,list<Move>, list<Move>::iterator);
    static void goToNextMove(ChessGame&, list<Move>, list<Move>::iterator);

    public:
    GameThread() = delete; // Delete constructor
    static void startGame();
};
