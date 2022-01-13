#include "GameThread.h"
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
using namespace sf;

constexpr unsigned int WINDOW_SIZE = 640;
constexpr unsigned int CELL_SIZE = WINDOW_SIZE / 8;
constexpr float SPRITE_SCALE = 0.6;
constexpr float SPRITE_SIZE = 128;


void GameThread::startGame() {
    ChessGame game;
    RenderWindow window(VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Chess Game", Style::Titlebar | Style::Close);

    // Setting window icon
    Image icon;
    icon.loadFromFile("./assets/icons/nw.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    window.setPosition(Vector2i(300,300));

    // Parameters to handle a piece being dragged
    bool pieceIsMoving = false;
    Piece* selectedPiece = nullptr;
    int xPos = 0, yPos = 0; // Mouse position
    int lastXPos = 0, lastYPos = 0; // Last position of the piece before being dragged
    moveTypes possibleMoves;

    // Additional board state variables
    Piece* lastMove = nullptr;

    // Sounds for piece movement
    SoundBuffer bufferMove;
    if (!bufferMove.loadFromFile("./assets/sounds/move.wav")) return;
    Sound soundMove;
    soundMove.setBuffer(bufferMove);

    SoundBuffer bufferCapture;
    if (!bufferCapture.loadFromFile("./assets/sounds/captures.wav")) return;
    Sound soundCapture;
    soundCapture.setBuffer(bufferCapture);
    
    
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

                    // for en passant
                    if(piece->getType() == PieceType::PAWN && lastMove != nullptr){
                        if(lastMove->getType() == PieceType::PAWN)
                            Pawn::setLastPawn((Pawn*) lastMove);
                    }

                    possibleMoves = game.possibleMovesFor(selectedPiece);
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
            if (event.type == Event::MouseButtonReleased) {
                if(event.mouseButton.button == Mouse::Left) {
                    // Should always be true by design
                    if (selectedPiece != nullptr) {
                        moveType* selectedMove = nullptr;

                        // Try to match moves
                        for (moveType& move: possibleMoves) {
                            if (get<0>(move).first == yPos/CELL_SIZE && get<0>(move).second == xPos/CELL_SIZE) {
                                selectedMove = &move;
                                break;
                            }
                        }

                        const int castleRow = (game.getTurn() == Team::WHITE)? 7: 0;

                        // If move is not allowed or king is checked, place piece back, else apply the move
                        if ((game.kingIsChecked() && selectedPiece->getType() != PieceType::KING) || selectedMove == nullptr) {
                            game.setBoardTile(lastXPos, lastYPos, selectedPiece);
                        } else {
                            switch (get<1>(*selectedMove)) {
                                case MoveType::NORMAL:
                                    game.setBoardTile(xPos/CELL_SIZE, yPos/CELL_SIZE, selectedPiece);
                                    // soundMove.play();
                                    break;
                                case MoveType::CAPTURE:
                                    game.setBoardTile(xPos/CELL_SIZE, yPos/CELL_SIZE, selectedPiece);
                                    // soundCapture.play();
                                    break;
                                case MoveType::ENPASSANT:
                                    game.setBoardTile(xPos/CELL_SIZE, yPos/CELL_SIZE, selectedPiece);
                                    Pawn::setLastPawn(nullptr);
                                    game.setBoardTile(lastMove->getY(), lastMove->getX(), nullptr);
                                    break;
                                case MoveType::CASTLE_KINGSIDE:
                                    game.setBoardTile(5, castleRow, game.getBoardTile(7, castleRow));
                                    game.setBoardTile(7, castleRow, nullptr);
                                    game.setBoardTile(6, castleRow, selectedPiece);
                                    break;
                                case MoveType::CASTLE_QUEENSIDE:
                                    game.setBoardTile(3, castleRow, game.getBoardTile(0, castleRow));
                                    game.setBoardTile(0, castleRow, nullptr);
                                    game.setBoardTile(2, castleRow, selectedPiece);
                                    break;
                                case MoveType::INIT_SPECIAL:
                                    game.setBoardTile(xPos/CELL_SIZE, yPos/CELL_SIZE, selectedPiece);
                                    break;
                                case MoveType::NEWPIECE:
                                    selectedPiece->move(-1, -1); // Deleted
                                    Queen* queen = new Queen(game.getTurn(), yPos/CELL_SIZE, xPos/CELL_SIZE);
                                    game.setBoardTile(xPos/CELL_SIZE, yPos/CELL_SIZE, queen);
                                    game.addPiece(queen);
                                    break;
                            }
                            lastMove = selectedPiece;
                            // lastMoveInitialXPos = lastXPos;
                            // lastMoveInitialYPos = lastYPos;
                            game.switchTurn();
                        }

                        xPos = 0; yPos = 0;
                        selectedPiece = nullptr;
                        pieceIsMoving = false;
                    }

                }

                if (event.mouseButton.button == Mouse::Right && selectedPiece != nullptr) {
                    // Reset the piece back
                    game.setBoardTile(lastXPos, lastYPos, selectedPiece);
                    selectedPiece = nullptr;
                    xPos = 0; yPos=0; 
                    pieceIsMoving = false;
                }
            }
        }

        initializeBoard(window);
        highlightLastMove(lastMove, window, lastXPos, lastYPos);
        drawPieces(window, game);

        if (pieceIsMoving) {
            drawCaptureCircles(window, possibleMoves, game);
            drawDraggedPiece(selectedPiece,window,xPos, yPos);
        }

        window.display();
    }
}

void GameThread::initializeBoard(RenderWindow &window) {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            // Drawing the colored square
            RectangleShape square(Vector2f(CELL_SIZE, CELL_SIZE));
            square.setFillColor(((i+j) % 2 == 0)? Color(181, 136, 99): Color(240, 217, 181));
            square.setPosition(i*CELL_SIZE, j*CELL_SIZE);
            window.draw(square);
        }
    }
}

void GameThread::drawCaptureCircles(RenderWindow &window, moveTypes &possibleMoves, ChessGame &game) {
    for (moveType& move: possibleMoves) {
        int j = get<0>(move).first;
        int i = get<0>(move).second;

        bool isEmpty = game.getBoardTile(i, j) == nullptr;
        Texture circleTexture;
        circleTexture.loadFromFile(isEmpty? "./assets/icons/circle.png": "./assets/icons/empty_circle.png");

        Sprite circle(circleTexture);
        if (isEmpty) circle.setScale(SPRITE_SCALE, SPRITE_SCALE);
        circle.setPosition(i*CELL_SIZE, j*CELL_SIZE);
        window.draw(circle);
    }
}

void GameThread::drawPieces(RenderWindow &window, ChessGame &game) {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (game.getBoardTile(i, j) != nullptr) {
                Texture t;
                t.loadFromFile(game.getBoardTile(i, j)->getFileName());
                Sprite tt(t);
                tt.setScale(SPRITE_SCALE, SPRITE_SCALE);
                tt.setPosition(i*CELL_SIZE, j*CELL_SIZE);
                window.draw(tt);
            }
        }
    }
}

void GameThread::drawDraggedPiece(Piece* selectedPiece, RenderWindow &window, int xPos, int yPos) {
    Texture t;
    t.loadFromFile(selectedPiece->getFileName());
    Sprite tt(t);
    tt.setScale(SPRITE_SCALE, SPRITE_SCALE);
    tt.setPosition(xPos, yPos);
    tt.setOrigin(SPRITE_SIZE/2, SPRITE_SIZE/2);
    window.draw(tt);
}

void GameThread::highlightLastMove(Piece* lastMove,RenderWindow &window, int lastXPos, int lastYPos) {  
    if(lastMove != nullptr) {
        RectangleShape squareBefore(Vector2f(CELL_SIZE, CELL_SIZE));
        RectangleShape squareAfter(Vector2f(CELL_SIZE, CELL_SIZE));
        squareBefore.setFillColor(((lastXPos + lastYPos) % 2 == 0)? Color(205, 210, 106): Color(170, 162, 58));
        squareAfter.setFillColor(((lastMove->getX() + lastMove->getY()) % 2 == 0)? Color(205, 210, 106): Color(170, 162, 58));

        squareBefore.setPosition(lastXPos*CELL_SIZE, lastYPos*CELL_SIZE);
        squareAfter.setPosition(lastMove->getY()*CELL_SIZE, lastMove->getX()*CELL_SIZE);

        window.draw(squareBefore);
        window.draw(squareAfter);

    }

}
