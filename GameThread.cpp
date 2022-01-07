#include "GameThread.h"

#include <SFML/Graphics.hpp>
#include <iostream>

#include "ChessGame.h"

using namespace sf;

void GameThread::startGame() {
    ChessGame* game = new ChessGame();

    RenderWindow window(VideoMode(640, 640),
                        "Chess Game");  // RenderWindow inherits Window class
    window.setVerticalSyncEnabled(
        true);  // prevent tearing, forces app to run at same FPS as monitor.
    Event event;

    // this is the main loop (a.k.a game loop) this ensures that the program
    // does not terminate until we exit.
    while (window.isOpen()) {
        //  we use a while loop for the pending events in case there were
        //  multiple events occured
        while (window.pollEvent(event))
            if (event.type == Event::Closed) { 
                window.close();
            } else if (event.type == Event::MouseButtonPressed) {
                if(event.mouseButton.button == Mouse::Left) {

                    // get the tile of the click
                    int xPos = event.mouseButton.x / 80; 
                    int yPos = event.mouseButton.y / 80; 

                    if(game->getBoardTile(i,j) != nullptr) {
                        
                    }
                }
            }

        // initializing the board
        
        window.clear(sf::Color::Black);
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                // drawing the colored square

                sf::RectangleShape square(sf::Vector2f(80, 80));

                if ((i + j) % 2 == 0) {
                    square.setFillColor(sf::Color(181, 136, 99));
                } else {
                    square.setFillColor(sf::Color(240, 217, 181));
                }
                square.setPosition(i * 80, j * 80);
                window.draw(square);

                // drawing the Piece if there is one

                if (game->_board[i][j] != nullptr) {
                    std::cout << "the current (row,col) is " << i << "," << j
                              << std::endl;
                    Texture t;
                    t.loadFromFile(game->getBoardTile(i, j)->_filename);
                    std::cout << "file name is "
                              << game->getBoardTile(i, j)->_filename
                              << std::endl;
                    Sprite tt(t);

                    tt.setScale(0.6, 0.6);
                    tt.setPosition(i * 80, j * 80);

                    window.draw(tt);
                }
            }
        }

        window.display();
    }
}