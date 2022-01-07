#include "GameThread.h"

#include <SFML/Graphics.hpp>
#include <iostream>

#include "ChessGame.h"

using namespace sf;

void GameThread::startGame() {
    
    ChessGame* game = new ChessGame();

    RenderWindow window(VideoMode(640, 640), "Chess Game");
    Event event;

    // initializing the board
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {

            // drawing the colored square

            sf::RectangleShape square(sf::Vector2f(80, 80));

            if ((i + j) % 2 == 0) {
                square.setFillColor(sf::Color(181, 136, 99));
                square.setOutlineColor(square.getFillColor());
            } else {
                square.setFillColor(sf::Color(240, 217, 181));
                square.setOutlineColor(square.getFillColor());
            }
            square.setPosition(i + 80, j + 80);
            window.draw(square);

            // drawing the Piece if there is one
            
            if(game->_board[i][j] != nullptr) {
                Texture t;
                t.loadFromFile(game->getBoardPiece(i,j)->_filename);
                Sprite tt(t);

                tt.setScale(0.6, 0.6);
                tt.setPosition(i+80, j+80);

                window.draw(tt);
            }
        }
    }
    window.display();

    while (window.isOpen()) {
        while (window.pollEvent(event))
            if (event.type == Event::Closed) window.close();

        window.display();
    }
}