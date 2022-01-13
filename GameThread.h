#pragma once
#include <SFML/Graphics.hpp>
#include "ChessGame.h"
using namespace sf;


class GameThread {    
    static void initializeBoard(RenderWindow&);
    static void drawCaptureCircles(RenderWindow&, moveTypes&, ChessGame&);
    static void drawPieces(RenderWindow&, ChessGame&);
    static void drawDraggedPiece(Piece*, RenderWindow&, int, int);
    static void highlightLastMove(Piece*, RenderWindow&, int, int);

    public:
    GameThread() = delete; // Delete constructor
    static void startGame();
};
