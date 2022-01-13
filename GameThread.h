#pragma once
#include <SFML/Graphics.hpp>
#include "Piece.h"
#include "ChessGame.h"


class GameThread {
    public:
    void startGame();
    
    void initializeBoard(sf::RenderWindow& window) const;
    void drawCaptureCircles(sf::RenderWindow &window, moveTypes &possibleMoves, ChessGame &game) const;
    void drawPieces(sf::RenderWindow &window, ChessGame &game) const;
    void drawDraggedPiece(Piece* selectedPiece, sf::RenderWindow &window, int xPos, int yPos) const;

};
