#include "../include/GameThread.hpp"
#include "../include/MoveList.hpp"
#include "../include/MenuButton.hpp"
#include <iostream>
#include <vector>
#include <list>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
using namespace sf;

void GameThread::startGame() {
    Board game;
    RenderWindow window(VideoMode(WINDOW_SIZE, WINDOW_SIZE + MENUBAR_HEIGHT), WINDOW_TITLE, Style::Titlebar | Style::Close);

    // Setting window icon
    Image icon;
    icon.loadFromFile(getIconPath("nw.png"));
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    window.setPosition({300, 300});

    // Window parameters
    vector<MenuButton> menuBar; 
    initializeMenuBar(menuBar);

    // Parameters to handle a piece being dragged
    bool pieceIsMoving = false;
    bool pieceIsClicked = false;
    Piece* selectedPiece = nullptr;
    coor2d mousePos = {0, 0};
    int lastXPos = 0, lastYPos = 0; // Last position of the piece before being dragged
    moveTypes possibleMoves;

    // Additional board state variables
    Piece* lastMove = nullptr;
    MoveList moveList(game);

    // Sounds for piece movement
    SoundBuffer bufferMove;
    if (!bufferMove.loadFromFile(getAudioPath("move.wav"))) return;
    Sound soundMove;
    soundMove.setBuffer(bufferMove);

    SoundBuffer bufferCapture;
    if (!bufferCapture.loadFromFile(getAudioPath("captures.wav"))) return;
    Sound soundCapture;
    soundCapture.setBuffer(bufferCapture);

    // This is the main loop (a.k.a game loop) this ensures that the program does not terminate until we exit
    Event event;
    while (window.isOpen()) {
        window.clear(Color(218, 224, 241));

        // We use a while loop for the pending events in case there were multiple events occured
        while (window.pollEvent(event)) {

            // Closing the window
            if (event.type == Event::Closed)
                window.close();

            // Clicking on a piece
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {

                // Get the tile of the click
                mousePos = {event.mouseButton.x, event.mouseButton.y};
                int yPos = getTileYPos(mousePos);
                if (yPos >= 0) {
                    if (event.mouseButton.y <= MENUBAR_HEIGHT) {
                        lastXPos = event.mouseButton.x; 
                        lastYPos = event.mouseButton.y; 
                    }
                    // Allow user to make moves only if they're at the current position, not looking at the previous played moves
                    if (moveList.hasMovesAfter()) continue;

                    if (pieceIsClicked) {
                        pieceIsMoving = true;
                        pieceIsClicked = false;
                        game.setBoardTile(lastXPos, lastYPos, nullptr, false); 
                        continue;
                    }
                    Piece* piece = game.getBoardTile(getTileXPos(mousePos), yPos);

                    // If piece is not null and has the right color
                    if (piece != nullptr && piece->getTeam() == game.getTurn()) {
                        selectedPiece = piece;
                        possibleMoves = game.possibleMovesFor(selectedPiece);

                        // Trim the illegal moves if in check
                        // check for absolute pin
                        removeIllegalMoves(game, possibleMoves, selectedPiece, mousePos);

                        pieceIsMoving = true;
                        lastXPos = getTileXPos(mousePos); lastYPos = yPos;
                        
                        // Set the tile on the board where the piece is selected to null
                        game.setBoardTile(lastXPos, lastYPos, nullptr, false); 
                    }
                } 
            }

            // Dragging a piece around
            if (event.type == Event::MouseMoved && (pieceIsMoving || pieceIsClicked)) {
                // Update the position of the piece that is being moved
                Vector2i MousePosition = Mouse::getPosition(window);
                mousePos = {MousePosition.x, MousePosition.y};
            }

            // Mouse button released
            if (event.type == Event::MouseButtonReleased) {
                if (event.mouseButton.button == Mouse::Left ) {
                    // if (mousePos.first == lastX && mousePos.second) {
                        
                    // }
                    if (selectedPiece == nullptr) return; 

                    // If clicked and mouse remained on the same square
                    if (getTileXPos(mousePos) == selectedPiece->getY() && getTileYPos(mousePos) == selectedPiece->getX()) {
                        if (!pieceIsClicked) {
                            game.setBoardTile(lastXPos, lastYPos, selectedPiece, false); 
                            pieceIsMoving = false;
                        }
                        pieceIsClicked = !pieceIsClicked;
                        continue;
                    }
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
                        game.setBoardTile(lastXPos, lastYPos, selectedPiece, false); // Cancel the move
                    } else {
                        moveList.addMove(
                            get<1>(*selectedMove), getTileXPos(mousePos), getTileYPos(mousePos),
                            lastXPos, lastYPos, selectedPiece, lastMove
                        );
                        lastMove = selectedPiece;
                        lastMove->setLastMove(get<1>(*selectedMove));
                        Piece::setLastMovedPiece(lastMove);
                        game.switchTurn();
                    }

                    selectedPiece = nullptr;
                    pieceIsMoving = false;
                    pieceIsClicked = false;
                    mousePos = {0, 0};
                }

                if (event.mouseButton.button == Mouse::Right && selectedPiece != nullptr && pieceIsMoving) {
                    // Reset the piece back
                    game.setBoardTile(lastXPos, lastYPos, selectedPiece, false);
                    selectedPiece = nullptr;
                    pieceIsMoving = false;
                }
            }

            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Left)
                    moveList.goToPreviousMove();
                else if (event.key.code == Keyboard::Right)
                    moveList.goToNextMove();
                else if (Keyboard::isKeyPressed(Keyboard::LControl) && Keyboard::isKeyPressed(Keyboard::F))
                    game.flipBoard();
            }
        }

        drawMenuBar(window, menuBar);
        initializeBoard(window, game);
        moveList.highlightLastMove(window);
        drawPieces(window, game);

        if (pieceIsClicked) {
            coor2d mousePosTemp = {Mouse::getPosition(window).x, Mouse::getPosition(window).y};
            drawCaptureCircles(window, possibleMoves, game);
            highlightHoveredSquare(window, game, possibleMoves, mousePosTemp);
        }
        if (pieceIsMoving) {
            drawCaptureCircles(window, possibleMoves, game);
            highlightHoveredSquare(window, game, possibleMoves, mousePos);
            drawDraggedPiece(selectedPiece,window, mousePos);
        }

        window.display();
    }
}
void GameThread::initializeMenuBar(vector<MenuButton>& menuBar) {
    constexpr uint16_t menuOptions = 3;
    const string iconFiles[menuOptions] = {"dropDown.png", "reset.png", "flip.png"};
    const string menuNames[menuOptions] = {"Menu", "Reset", "Flip"};
    Font font; 
    if (!font.loadFromFile("../assets/fonts/Arial.ttf")) return;

    for(uint8_t i = 0; i <menuOptions; ++i){
        menuBar.push_back(MenuButton(i, false, font, menuNames[i], iconFiles[i]));
    }
}
void GameThread::drawMenuBar(RenderWindow& window, vector<MenuButton>& menuBar) {
 
    for(vector<MenuButton>::iterator it = begin(menuBar); it != end(menuBar); ++it) {
        window.draw((*it).getRectangle());
        window.draw((*it).getSprite());
        // window.draw((*it).getText());
    }   
}

void GameThread::removeIllegalMoves(Board& game, moveTypes& possibleMoves, Piece* selectedPiece, coor2d& mousePos) {
    moveTypes::iterator it = possibleMoves.begin();

    while (it != possibleMoves.end()) {
        int x = get<0>(*it).second, y = get<0>(*it).first;

        // Store piece occupied by target square
        Piece* temp = game.getBoardTile(x, y);

        game.setBoardTile(x, y, selectedPiece, false); // Move this piece to target square
        game.setBoardTile(getTileXPos(mousePos), getTileYPos(mousePos), nullptr, false); // Set null to selected piece's square

        if (game.kingIsChecked()) it = possibleMoves.erase(it);
        else ++it;

        game.setBoardTile(getTileXPos(mousePos), getTileYPos(mousePos), selectedPiece, false);
        game.setBoardTile(x, y, temp, false); 
    }
}

void GameThread::initializeBoard(RenderWindow& window, Board& game) {
    const Color colours[2] = {{240, 217, 181}, {181, 136, 99}};

    for (uint8_t i = 0; i < 8; ++i) {
        for (uint8_t j = 0; j < 8; ++j) {
            // Drawing the colored square
            RectangleShape square(Vector2f(CELL_SIZE, CELL_SIZE));
            square.setFillColor(colours[(i+j)%2 ^ game.isFlipped()]);
            square.setPosition(getWindowXPos(i), getWindowYPos(j));
            window.draw(square);
        }
    }
}

void GameThread::highlightHoveredSquare(RenderWindow& window, Board& game, moveTypes& possibleMoves, coor2d& mousePos) {
    const Color colours[2] = {{173, 176, 134}, {100, 111, 64}};

    for (moveType& move: possibleMoves) {
        int i = get<0>(move).second, j = get<0>(move).first;
        int xPos = getTileXPos(mousePos), yPos = getTileYPos(mousePos);

        if (i == xPos && j == yPos) {
            // Currently hovering a square where the piece can move 
            RectangleShape square({CELL_SIZE, CELL_SIZE});
            square.setFillColor(colours[(i+j)%2 ^ game.isFlipped()]);
            square.setPosition(getWindowXPos(i), getWindowYPos(j));
            window.draw(square);
        }
    }
}

void GameThread::drawCaptureCircles(RenderWindow& window, moveTypes& possibleMoves, Board& game) {
    for (moveType& move: possibleMoves) {
        int i = get<0>(move).second, j = get<0>(move).first;

        bool isEmpty = game.getBoardTile(i, j) == nullptr;
        Texture circleTexture;
        circleTexture.loadFromFile(getIconPath(isEmpty? "circle.png": "empty_circle.png"));

        Sprite circle(circleTexture);
        if (isEmpty) circle.setScale(SPRITE_SCALE, SPRITE_SCALE);
        circle.setPosition(getWindowXPos(i), getWindowYPos(j));

        window.draw(circle);
    }
}

void GameThread::drawPieces(RenderWindow& window, Board& game) {
    for (uint8_t i = 0; i < 8; ++i) {
        for (uint8_t j = 0; j < 8; ++j) {
            if (game.getBoardTile(i, j) != nullptr) {
                Texture t;
                t.loadFromFile(game.getBoardTile(i, j)->getFileName());
                Sprite s(t);
                s.setScale(SPRITE_SCALE, SPRITE_SCALE);
                s.setPosition(getWindowXPos(i), getWindowYPos(j));
                window.draw(s);
            }
        }
    }
}

void GameThread::drawDraggedPiece(Piece* selectedPiece, RenderWindow& window, coor2d& mousePos) {
    Texture t;
    t.loadFromFile(selectedPiece->getFileName());
    Sprite s(t);
    s.setScale(SPRITE_SCALE, SPRITE_SCALE);
    s.setPosition(mousePos.first, mousePos.second);
    s.setOrigin(SPRITE_SIZE/2, SPRITE_SIZE/2);
    window.draw(s);
}
