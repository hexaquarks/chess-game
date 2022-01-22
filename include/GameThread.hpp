#pragma once
#include <SFML/Graphics.hpp>
#include "Board.hpp"
using namespace sf;

typedef pair<int, int> coor2d;

class GameThread {    
    static void initializeBoard(RenderWindow&, Board&);
    static void drawCaptureCircles(RenderWindow&, moveTypes&, Board&);
    static void highlightHoveredSquare(RenderWindow&, Board&, moveTypes&, int, int);
    static void drawPieces(RenderWindow&, Board&);
    static void drawDraggedPiece(Piece*, RenderWindow&, int, int);
    static void removeIllegalMoves(Board&, moveTypes&, Piece*, coor2d&);

    public:
    GameThread() = delete; // Delete constructor
    static void startGame();
    static int getTileXPos(coor2d& pos) { return getTileXPos(pos.first); }
    static int getTileXPos(int xMousePos) { return xMousePos / CELL_SIZE; }
    static int getTileYPos(coor2d& pos) { return getTileYPos(pos.second); }
    static int getTileYPos(int yMousePos) { return yMousePos / CELL_SIZE; }
};
