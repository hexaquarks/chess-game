#include "GameThread.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "ChessGame.h"
using namespace sf;

constexpr unsigned int WINDOW_SIZE = 1280;
constexpr unsigned int CELL_SIZE = WINDOW_SIZE / 8;


void GameThread::startGame() {
    ChessGame game;
    RenderWindow window(VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Chess Game", Style::Titlebar | Style::Close);

    // Setting window icon
    Image icon;
    icon.loadFromFile("./assets/nw.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    // Parameters to handle a piece being dragged
    bool pieceIsMoving;
    Piece* selectedPiece = nullptr;
    int xPos = 0, yPos = 0; // Mouse position
    int lastXPos = 0, lastYPos = 0; // Last position of the piece before being dragged

    // This is the main loop (a.k.a game loop) this ensures that the program does not terminate until we exit
    Event event;
    while (window.isOpen()) {
        window.clear(Color::Black);

        // We use a while loop for the pending events in case there were multiple events occured
        while (window.pollEvent(event)) {

            // Closing the window
            if (event.type == Event::Closed)
                window.close();

            // Clicking on a piece
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                // Get the tile of the click
                xPos = event.mouseButton.x;
                yPos = event.mouseButton.y;
                Piece* piece = game.getBoardTile(xPos/CELL_SIZE, yPos/CELL_SIZE);

                // If piece is not null and has the right color
                if (piece != nullptr && piece->getTeam() == game.getTurn()) {
                    selectedPiece = piece;
                    pieceIsMoving = true;
                    lastXPos = xPos/CELL_SIZE; lastYPos = yPos/CELL_SIZE;
                    game.setBoardTile(lastXPos, lastYPos, nullptr); // Set the tile on the board where the piece is selected to null
                }
            }

            // Dragging a piece around
            if (event.type == Event::MouseMoved && pieceIsMoving) {
                // Update the position of the piece that is being moved
                Vector2i MousePosition = Mouse::getPosition(window);
                xPos = MousePosition.x;
                yPos = MousePosition.y;
            }

            // Mouse button released
            if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left && selectedPiece != nullptr) {
                moveTypes possibleMoves = game.possibleMovesFor(selectedPiece);
                moveType* selectedMove = nullptr;

                // Try to match moves
                for (moveType& move: possibleMoves) {
                    if (get<0>(move).first == xPos/CELL_SIZE && get<0>(move).second == yPos/CELL_SIZE) {
                        selectedMove = &move;
                        break;
                    }
                }

                // If move is not allowed, place piece back, else apply the move
                if (selectedMove == nullptr) {
                    game.setBoardTile(lastXPos, lastYPos, selectedPiece);
                } else {
                    switch (get<1>(*selectedMove)) {
                        case MoveType::NORMAL:
                            game.setBoardTile(xPos/CELL_SIZE, yPos/CELL_SIZE, selectedPiece);
                            break;
                        case MoveType::CAPTURE:
                            game.setBoardTile(xPos/CELL_SIZE, yPos/CELL_SIZE, selectedPiece);
                            break;
                        case MoveType::ENPASSANT:
                            break;
                        case MoveType::NEWPIECE:
                            break;
                        case MoveType::CASTLE:
                            break;
                    }
                    game.switchTurn();
                }

                xPos = 0; yPos = 0;
                selectedPiece = nullptr;
                pieceIsMoving = false;
        }
    }

    // Initializing the board
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            // Drawing the colored square
            RectangleShape square(Vector2f(CELL_SIZE, CELL_SIZE));
            square.setFillColor(((i+j) % 2 == 0)? Color(181, 136, 99): Color(240, 217, 181));
            square.setPosition(i*CELL_SIZE, j*CELL_SIZE);
            window.draw(square);

            // Drawing the Piece if there is one
            if (game.getBoardTile(i, j) != nullptr) {
                Texture t;
                t.loadFromFile(game.getBoardTile(i, j)->getFileName());
                Sprite tt(t);
                tt.setScale(1.2, 1.2);
                tt.setPosition(i*CELL_SIZE, j*CELL_SIZE);
                window.draw(tt);
            }
        }
    }

        // Draw the piece that is being dragged
        if (pieceIsMoving) {
            Texture t;
            t.loadFromFile(selectedPiece->getFileName());
            Sprite tt(t);
            tt.setScale(1.2, 1.2);
            tt.setPosition(xPos, yPos);
            tt.setOrigin(64, 64);
            window.draw(tt);
        }

        window.display();
    }
}
