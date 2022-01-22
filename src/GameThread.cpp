#include "../include/GameThread.hpp"
#include "../include/MoveList.hpp"
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <list>
using namespace sf;

constexpr uint32_t MENUBAR_HEIGHT = 60;
constexpr float SPRITE_SCALE = 0.6;
constexpr float SPRITE_SIZE = 128;


void GameThread::startGame() {
    Board game;
    RenderWindow window(VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Chess Game", Style::Titlebar | Style::Close);
    // View view(FloatRect(0, 0, WINDOW_SIZE, WINDOW_SIZE+MENUBAR_HEIGHT));
    // view.move(0.f, -60.f);
    // window.setView(view);


    // Setting window icong
    Image icon;
    icon.loadFromFile("../assets/icons/nw.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    window.setPosition(Vector2i(300,300));

    // Parameters to handle a piece being dragged
    bool pieceIsMoving = false;
    Piece* selectedPiece = nullptr;
    coor2d mousePos = {0, 0};
    int lastXPos = 0, lastYPos = 0; // Last position of the piece before being dragged
    moveTypes possibleMoves;

    // Additional board state variables
    Piece* lastMove = nullptr;
    MoveList moveList(game);

    // Sounds for piece movement
    SoundBuffer bufferMove;
    if (!bufferMove.loadFromFile("../assets/sounds/move.wav")) return;
    Sound soundMove;
    soundMove.setBuffer(bufferMove);

    SoundBuffer bufferCapture;
    if (!bufferCapture.loadFromFile("../assets/sounds/captures.wav")) return;
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

                // Allow user to make moves only if they're at the current position, not looking at the previous played moves
                if (moveList.hasMovesAfter()) continue;

                // Get the tile of the click
                mousePos = {event.mouseButton.x, event.mouseButton.y};
                Piece* piece = game.getBoardTile(getTileXPos(mousePos), getTileYPos(mousePos));

                // If piece is not null and has the right color
                if (piece != nullptr && piece->getTeam() == game.getTurn()) {
                    selectedPiece = piece;
                    possibleMoves = game.possibleMovesFor(selectedPiece);

                    // Trim the illegal moves if in check
                    // check for absolute pin
                    removeIllegalMoves(game, possibleMoves, selectedPiece, mousePos);

                    pieceIsMoving = true;
                    lastXPos = getTileXPos(mousePos); lastYPos = getTileYPos(mousePos);
                    game.setBoardTile(lastXPos, lastYPos, nullptr, false); // Set the tile on the board where the piece is selected to null
                }
            }

            // Dragging a piece around
            if (event.type == Event::MouseMoved && pieceIsMoving) {
                // Update the position of the piece that is being moved
                Vector2i MousePosition = Mouse::getPosition(window);
                mousePos = {MousePosition.x, MousePosition.y};
            }

            // Mouse button released
            if (event.type == Event::MouseButtonReleased) {
                if(event.mouseButton.button == Mouse::Left) {
                    // Should always be true by design
                    if (selectedPiece != nullptr) {
                        moveType* selectedMove = nullptr;

                        // Try to match moves
                        for (moveType& move: possibleMoves) {
                            if (get<0>(move).first == getTileYPos(mousePos) && get<0>(move).second == getTileXPos(mousePos)) {
                                selectedMove = &move;
                                break;
                            }
                        }

                        // If move is not allowed or king is checked, place piece back, else apply the move
                        if (selectedMove == nullptr) {
                            game.setBoardTile(lastXPos, lastYPos, selectedPiece, false); // cancel the move
                        } else {
                            moveList.addMove(get<1>(*selectedMove), getTileXPos(mousePos.first), getTileYPos(mousePos.second), lastXPos, lastYPos, selectedPiece, lastMove);
                            lastMove = selectedPiece;
                            lastMove->setLastMove(get<1>(*selectedMove));
                            Piece::setLastMovedPiece(lastMove);
                            game.switchTurn();
                        }

                        mousePos = {0, 0};
                        selectedPiece = nullptr;
                        pieceIsMoving = false;
                    }
                }

                if (event.mouseButton.button == Mouse::Right && selectedPiece != nullptr) {
                    // Reset the piece back
                    game.setBoardTile(lastXPos, lastYPos, selectedPiece, false);
                    selectedPiece = nullptr;
                    pieceIsMoving = false;
                }
            }

            if (event.type == Event::KeyPressed)  {
                if (event.key.code == Keyboard::Left)
                    moveList.goToPreviousMove();
                else if (event.key.code == Keyboard::Right)
                    moveList.goToNextMove();
                else if (Keyboard::isKeyPressed(Keyboard::LControl) && Keyboard::isKeyPressed(Keyboard::F))
                    game.flipBoard();
            }
        }

        initializeBoard(window, game);
        moveList.highlightLastMove(window);
        drawPieces(window, game);

        if (pieceIsMoving) {
            drawCaptureCircles(window, possibleMoves, game);
            highlightHoveredSquare(window, game, possibleMoves, mousePos.first, mousePos.second);
            drawDraggedPiece(selectedPiece,window, mousePos.first, mousePos.second);
        }

        window.display();
    }
}



void GameThread::removeIllegalMoves(Board& game, moveTypes& possibleMoves, Piece* selectedPiece, coor2d& mousePos) {
    moveTypes::iterator it = possibleMoves.begin();
    while (it != possibleMoves.end()) {
        int y = get<0>(*it).first;
        int x = get<0>(*it).second; 

        // Store piece occupied by target square
        Piece* temp = game.getBoardTile(x,y);

        game.setBoardTile(x, y, selectedPiece, false); // move this piece to target square
        game.setBoardTile(getTileXPos(mousePos), getTileYPos(mousePos), nullptr, false); // set null to selected piece's square

        if (game.kingIsChecked()) it = possibleMoves.erase(it);
        else ++it;

        game.setBoardTile(getTileXPos(mousePos), getTileYPos(mousePos), selectedPiece, false);
        game.setBoardTile(x, y, temp, false); 
    }
}

void GameThread::initializeBoard(RenderWindow& window, Board& game) {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            // Drawing the colored square
            RectangleShape square(Vector2f(CELL_SIZE, CELL_SIZE));
            square.setFillColor(((i+j) % 2 != 0)
                ? game.isFlipped() ? Color(240, 217, 181) : Color(181, 136, 99) 
                : game.isFlipped() ? Color(181, 136, 99) : Color(240, 217, 181));
            square.setPosition(i*CELL_SIZE, j*CELL_SIZE);
            window.draw(square);
        }
    }
}
void GameThread::highlightHoveredSquare(RenderWindow& window, Board& game, moveTypes& possibleMoves, int xPos, int yPos) {
    for (moveType& move: possibleMoves){
        int i = get<0>(move).second;
        int j = get<0>(move).first;
        if (i == xPos/CELL_SIZE && j == yPos/CELL_SIZE) {
            // Currently hovering a square where the piece can move 
            RectangleShape square(Vector2f(CELL_SIZE, CELL_SIZE));
            square.setFillColor((i + j) % 2 != 0 
                ? game.isFlipped() ? Color(173,176,134) : Color(100, 111, 64) 
                : game.isFlipped() ? Color(100, 111, 64)  : Color(173,176,134));
            square.setPosition(i * CELL_SIZE, j * CELL_SIZE);
            window.draw(square);
        }
    }

};

void GameThread::drawCaptureCircles(RenderWindow& window, moveTypes& possibleMoves, Board& game) {
    for (moveType& move: possibleMoves) {
        int j = get<0>(move).first;
        int i = get<0>(move).second;

        bool isEmpty = game.getBoardTile(i, j) == nullptr;
        Texture circleTexture;
        circleTexture.loadFromFile(isEmpty? "../assets/icons/circle.png": "../assets/icons/empty_circle.png");

        Sprite circle(circleTexture);
        if (isEmpty) circle.setScale(SPRITE_SCALE, SPRITE_SCALE);
        circle.setPosition(i*CELL_SIZE, j*CELL_SIZE);

        window.draw(circle);
    }
}

void GameThread::drawPieces(RenderWindow& window, Board& game) {
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

void GameThread::drawDraggedPiece(Piece* selectedPiece, RenderWindow& window, int xPos, int yPos) {
    Texture t;
    t.loadFromFile(selectedPiece->getFileName());
    Sprite tt(t);
    tt.setScale(SPRITE_SCALE, SPRITE_SCALE);
    tt.setPosition(xPos, yPos);
    tt.setOrigin(SPRITE_SIZE/2, SPRITE_SIZE/2);
    window.draw(tt);
}
