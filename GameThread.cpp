#include "GameThread.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "ChessGame.h"
using namespace sf;

void GameThread::startGame() {
    ChessGame game;
    RenderWindow window(VideoMode(640, 640), "Chess Game", Style::Titlebar | Style::Close);

    // Setting window icon
    Image icon;
    icon.loadFromFile("./assets/nw.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    // Parameters to handle a piece being dragged
    bool pieceIsMoving;
    Piece* selectedPiece;
    int xPos = 0, yPos = 0;

    // This is the main loop (a.k.a game loop) this ensures that the program
    // does not terminate until we exit.
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

                if (game.getBoardTile(xPos / 80, yPos / 80) != nullptr) {
                    // check if the selected piece's color corresponds to
                    // the player's turn color
                    if (game.getBoardTile(xPos / 80, yPos / 80)->getTeam() == game.getTurn()) {
                        selectedPiece = game.getBoardTile(xPos / 80, yPos / 80);
                        pieceIsMoving = true;

                        // set the tile on the board where the piece is
                        // selected, to null and draw the dragged piece
                        // explicitly
                        game.setBoardTile(xPos / 80, yPos / 80, nullptr);
                    }
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
                moveType selectedMove;

                for (const auto& move: possibleMoves) 
                    if (get<0>(move).first == xPos/80 && get<0>(move).second == yPos/80) 
                        selectedMove = move;

                // Apply the move
                switch (get<1>(selectedMove)) {
                    case MoveType::NORMAL :
                        game.setBoardTile(xPos/80, yPos/80, selectedPiece);
                        break;
                    case MoveType::CAPTURE :
                        game.setBoardTile(xPos/80, yPos/80, selectedPiece);
                        break;
                    case MoveType::ENPASSANT :
                        break;
                    case MoveType::NEWPIECE :
                        break;
                    case MoveType::CASTLE :
                        break;
                }

                // if (game.getBoardTile(xPos / 80, yPos / 80) == nullptr) {
                //     // drop a piece to the tile only if is empty (for
                //     // testing)
                //     game.setBoardTile(xPos / 80, yPos / 80, selectedPiece);
                // }

                xPos = -1;
                yPos = -1;
                selectedPiece = nullptr;
                pieceIsMoving = false;
                game.switchTurn();
        }
    }

    // Initializing the board
    for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                // Drawing the colored square
                RectangleShape square(Vector2f(80, 80));

                square.setFillColor(((i+j) % 2 == 0)? Color(181, 136, 99): Color(240, 217, 181));
                square.setPosition(i * 80, j * 80);
                window.draw(square);

                // Drawing the Piece if there is one
                if (game.getBoardTile(i, j) != nullptr) {
                    Texture t;
                    t.loadFromFile(game.getBoardTile(i, j)->getFileName());
                    Sprite tt(t);
                    tt.setScale(0.6, 0.6);
                    tt.setPosition(i * 80, j * 80);
                    window.draw(tt);
                }
            }
        }

        // Draw the piece that is being dragged
        if (pieceIsMoving) {
            Texture t;
            t.loadFromFile(selectedPiece->getFileName());
            Sprite tt(t);
            tt.setScale(0.6, 0.6);
            tt.setPosition(xPos, yPos);
            tt.setOrigin(64, 64);
            window.draw(tt);
        }

        window.display();
    }
}
