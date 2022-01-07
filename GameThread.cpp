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

    // parameters to handle a piece being dragged
    bool pieceIsMoving;
    Piece* selectedPiece;
    int xPos;
    int yPos;

    // this is the main loop (a.k.a game loop) this ensures that the program
    // does not terminate until we exit.
    while (window.isOpen()) {
        window.clear(sf::Color::Black);
        //  we use a while loop for the pending events in case there were
        //  multiple events occured
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {

                if (event.mouseButton.button == sf::Mouse::Left) {
                    // get the tile of the click
                    xPos = event.mouseButton.x;
                    yPos = event.mouseButton.y;

                    if (game->getBoardTile(xPos / 80, yPos / 80) != nullptr) {
                        std::cout << "Succesfully clicked a piece" << std::endl;
                        selectedPiece = game->getBoardTile(xPos, yPos);
                        pieceIsMoving = true;

                        // set the tile on the board where the piece is selected, to
                        // null and draw the dragged piece explicitly
                        game->_board[xPos / 80][yPos / 80] = nullptr;
                    }

                }
            }

            if (event.type == sf::Event::MouseMoved && pieceIsMoving) {
                // update the position of the piece that is being moved
                xPos = event.mouseButton.x;
                yPos = event.mouseButton.y;
            }
            if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (game->getBoardTile(xPos / 80, yPos / 80) == nullptr) {
                        // drop a piece to the tile only if is empty (for
                        // testing)
                        game->_board[xPos / 80][yPos / 80] = selectedPiece;
                    }

                    xPos = -1;
                    yPos = -1;
                    selectedPiece = nullptr;
                    pieceIsMoving = false;
                }
            }
        }
        // initializing the board

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
                    Texture t;
                    t.loadFromFile(game->getBoardTile(i, j)->_filename);
                    Sprite tt(t);

                    tt.setScale(0.6, 0.6);
                    tt.setPosition(i * 80, j * 80);

                    window.draw(tt);
                }

                // draw the piece that is being dragged
                if (pieceIsMoving) {
                    Texture t;
                    t.loadFromFile(selectedPiece->_filename);
                    Sprite tt(t);
                    tt.setScale(0.6, 0.6);
                    tt.setPosition(xPos, yPos);
                    window.draw(tt);

                    std::cout << "Succesfully drawed the new piece"
                              << std::endl;
                }
            }
        }

        window.display();
    }
}