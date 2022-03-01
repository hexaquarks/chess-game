#include "../include/GameThread.hpp"
#include "../include/MoveList.hpp"
#include "../include/MenuButton.hpp"
#include "../include/RessourceManager.hpp"
#include "../include/PieceTransition.hpp"
#include "../include/Move.hpp"

#include <iostream>
#include <vector>
#include <list>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

using namespace sf;

void GameThread::startGame() {
    Board game;
    RenderWindow window(VideoMode(WINDOW_SIZE, WINDOW_SIZE + MENUBAR_HEIGHT), WINDOW_TITLE, Style::Titlebar | Style::Close);

    // Load ressources 
    RessourceManager::loadRessources();

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
    bool isRightClicking = false;
    Piece* selectedPiece = nullptr;
    Piece* movingPiece = nullptr; // Piece transition
    coor2d mousePos = {0, 0};
    coor2d rightClickAnchor = {0, 0};
    int lastXPos = 0, lastYPos = 0; // Last position of the piece before being dragged
    vector<Move> possibleMoves = game.calculateAllMoves();

    // Additional board state variables
    Piece* lastMove = nullptr;
    PieceTransition transitioningPiece;
    transitioningPiece.setTransitioningPiece();
    MoveList moveList(game, transitioningPiece);
    vector<Arrow> arrowList;
    Arrow arrow;

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
            if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Left) {
                    // Get the tile of the click
                    mousePos = {event.mouseButton.x, event.mouseButton.y};

                    // Allow user to make moves only if they're at the current live position, 
                    // and if the click is on the chess board
                    int yPos = getTileYPos(mousePos);
                    if (yPos < 0 || moveList.hasMovesAfter()) continue;

                    int xPos = isFlipped? 7-getTileXPos(mousePos): getTileXPos(mousePos);
                    if (isFlipped) yPos = 7-yPos;
                    Piece* piece = game.getBoardTile(xPos, yPos);

                    // If piece is not null and has the right color
                    if (piece != nullptr && piece->getTeam() == game.getTurn()) {
                        // Unselect clicked piece
                        if(piece == selectedPiece) {
                            selectedPiece = nullptr;
                            pieceIsClicked = false;
                            continue;
                        }

                        selectedPiece = piece;

                        // Trim the illegal moves if in check
                        // Check for absolute pin
                        // removeIllegalMoves(game, possibleMoves, selectedPiece, mousePos);
                        pieceIsMoving = true;
                        pieceIsClicked = false;
                        lastXPos = isFlipped? 7-getTileXPos(mousePos): getTileXPos(mousePos); lastYPos = yPos;

                        // Set the tile on the board where the piece is selected to null
                        game.setBoardTile(lastXPos, lastYPos, nullptr, false); 
                    }
                }
                if (event.mouseButton.button == Mouse::Right) {
                    rightClickAnchor = {event.mouseButton.x, event.mouseButton.y};
                    arrow.setOrigin(rightClickAnchor);
                    arrow.setDestination(rightClickAnchor);
                    isRightClicking = true;
                }
            }

            // Dragging a piece around
            if (event.type == Event::MouseMoved && (pieceIsMoving || pieceIsClicked || isRightClicking)) {
                // Update the position of the piece that is being moved
                Vector2i mousePosition = Mouse::getPosition(window);
                mousePos = {mousePosition.x, mousePosition.y};

                if (isRightClicking) {
                    arrow.setDestination(mousePos);
                    arrow.updateArrow(); // update the type and rotation
                }
            }

            // Mouse button released
            if (event.type == Event::MouseButtonReleased) {
                if (event.mouseButton.button == Mouse::Left) {
                    [[unlikely]] if (mousePos.second < MENUBAR_HEIGHT)
                        for (MenuButton& m: menuBar)
                            if (m.isClicked(mousePos))
                                if(m.performClick(game, moveList) == 1 && selectedPiece != nullptr) {
                                    // possibleMoves = game.possibleMovesFor(selectedPiece);
                                    // removeIllegalMoves(game, possibleMoves, selectedPiece, mousePos);
                                    selectedPiece = nullptr;
                                    mousePos = {0, 0};
                                }

                    if (selectedPiece == nullptr) continue;

                    // If clicked and mouse remained on the same square
                    int xPos = isFlipped? 7-getTileXPos(mousePos): getTileXPos(mousePos);
                    int yPos = isFlipped? 7-getTileYPos(mousePos): getTileYPos(mousePos);
                    if (xPos == selectedPiece->getY() && yPos == selectedPiece->getX()) {
                        if (!pieceIsClicked) {
                            // Put the piece back to it's square; it's not moving
                            game.setBoardTile(lastXPos, lastYPos, selectedPiece, false); 
                            pieceIsMoving = false;
                        }
                        pieceIsClicked = !pieceIsClicked;
                        continue;
                    }

                    // Try to match moves
                    Move* selectedMove = nullptr;
                    for (auto& move: possibleMoves) {
                        if (move.getSelectedPiece() == selectedPiece) {
                            if (move.getTarget().first == yPos && move.getTarget().second == xPos) {
                                selectedMove = &move;
                                break;
                            }
                        }
                    }

                    // If move is not allowed or king is checked, place piece back, else apply the move
                    if (selectedMove == nullptr) {
                        game.setBoardTile(lastXPos, lastYPos, selectedPiece, false); // Cancel the move
                    } else {
                        coor2d target = make_pair(xPos, yPos);
                        coor2d initial = make_pair(lastXPos, lastYPos);

                        Move move(target, initial, selectedPiece, selectedMove->getMoveType());
                        move.setCapturedPiece(lastMove);
                        moveList.addMove(move);

                        lastMove = selectedPiece;
                        lastMove->setLastMove(selectedMove->getMoveType());
                        Piece::setLastMovedPiece(lastMove);
                        game.switchTurn();
                        possibleMoves = game.calculateAllMoves();
                    }

                    selectedPiece = nullptr;
                    pieceIsMoving = false;
                    pieceIsClicked = false;
                    mousePos = {0, 0};
                }
                if (event.mouseButton.button == Mouse::Right) {
                    if (selectedPiece != nullptr && pieceIsMoving) {
                        // Reset the piece back
                        game.setBoardTile(lastXPos, lastYPos, selectedPiece, false);
                        selectedPiece = nullptr;
                        pieceIsMoving = false;
                    } else if (isRightClicking) {
                        // add arrow to arrow list to be drawn
                        if (arrow.isDrawable()) {
                            if (!arrow.removeArrow(arrowList)) arrowList.push_back(arrow);
                        }
                        isRightClicking = false;
                        rightClickAnchor = {0,0};
                        arrow.resetParameters();
                    }
                }
            }

            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Left && !transitioningPiece.getIsTransitioning()) 
                    moveList.goToPreviousMove(true);
                else if (event.key.code == Keyboard::Right && !transitioningPiece.getIsTransitioning())
                    moveList.goToNextMove(true);
                else if (Keyboard::isKeyPressed(Keyboard::LControl) && Keyboard::isKeyPressed(Keyboard::F))
                    flipBoard();
                else if (event.key.code == Keyboard::Up)
                    moveList.goToCurrentMove();
                else if (event.key.code == Keyboard::Down)
                    moveList.goToInitialMove();
            }
        }

        drawMenuBar(window, menuBar);
        initializeBoard(window, game);
        moveList.highlightLastMove(window);

        if ((pieceIsMoving || pieceIsClicked) && selectedPiece != nullptr) {
            drawCaptureCircles(window, possibleMoves, selectedPiece, game);
            highlightHoveredSquare(window, selectedPiece, game, possibleMoves, mousePos);
        }
        drawPieces(window, game);
        if (pieceIsMoving) drawDraggedPiece(selectedPiece, window, mousePos);
        if (transitioningPiece.getIsTransitioning()) {
            drawTransitioningPiece(window, transitioningPiece, game);
        }

        if (arrow.isDrawable()) drawCurrentArrow(window, arrow);
        drawAllArrows(window, arrowList);
        window.display();
    }
}

void GameThread::initializeMenuBar(vector<MenuButton>& menuBar) {
    constexpr uint16_t menuOptions = 3;
    const string menuNames[menuOptions] = {"Menu", "Reset", "Flip"};

    for (uint8_t i = 0; i < menuOptions; ++i) menuBar.push_back(MenuButton(i, menuNames[i]));
}

void GameThread::drawMenuBar(RenderWindow& window, vector<MenuButton>& menuBar) {
    constexpr uint16_t menuOptions = 3;
    const string iconFiles[menuOptions] = {"dropDown.png", "reset.png", "flip.png"};

    for (uint8_t i = 0; i < menuOptions; ++i) {
        shared_ptr<Texture> t = RessourceManager::getTexture(iconFiles[i]);
        MenuButton& option = menuBar[i];
        option.setSpriteTexture(*t);

        // handle color transition of menu button when clicked
        if (option.getIsColorTransitioning()) option.doColorTransition();
        
        option.drawMenuButton(window);
    }   
}

void GameThread::initializeBoard(RenderWindow& window, Board& game) {
    const Color colours[2] = {{240, 217, 181}, {181, 136, 99}};

    for (uint8_t i = 0; i < 8; ++i) {
        for (uint8_t j = 0; j < 8; ++j) {
            // Drawing the colored square
            RectangleShape square = createSquare();
            square.setFillColor(colours[(i+j)%2]);
            square.setPosition(getWindowXPos(i), getWindowYPos(j));
            window.draw(square);
        }
    }
}

void GameThread::highlightHoveredSquare(RenderWindow& window, Piece* selectedPiece, Board& game, vector<Move>& possibleMoves, coor2d& mousePos) {
    const Color colours[2] = {{173, 176, 134}, {100, 111, 64}};

    for (Move& move: possibleMoves) {
        int i = move.getTarget().second;
        int j = move.getTarget().first;
        if (isFlipped) {i = 7-i; j = 7-j;}
        if (move.getSelectedPiece() != selectedPiece) continue;
        int xPos = getTileXPos(mousePos), yPos = getTileYPos(mousePos);

        if (i == xPos && j == yPos) {
            // Currently hovering a square where the piece can move 
            RectangleShape square = createSquare();
            square.setFillColor(colours[(i+j)%2]);
            square.setPosition(getWindowXPos(i), getWindowYPos(j));
            window.draw(square);
        }
    }
}

void GameThread::drawCaptureCircles(RenderWindow& window, vector<Move>& possibleMoves, Piece* selectedPiece, Board& game) {
    for (Move& move: possibleMoves) {
        int i = move.getTarget().second;
        int j = move.getTarget().first;
        if (isFlipped) {i = 7-i; j = 7-j;}
        if (move.getSelectedPiece() != selectedPiece) continue;
        bool isEmpty = game.getBoardTile(i, j) == nullptr;
        shared_ptr<Texture> t = RessourceManager::getTexture(isEmpty? "circle.png": "empty_circle.png");
        if (t == nullptr) return;
        Sprite circle(*t);
        if (isEmpty) circle.setScale(SPRITE_SCALE, SPRITE_SCALE);
        circle.setPosition(getWindowXPos(i), getWindowYPos(j));
        window.draw(circle);
    }
}

void GameThread::drawPieces(RenderWindow& window, Board& game) {
    for (uint8_t i = 0; i < 8; ++i) {
        for (uint8_t j = 0; j < 8; ++j) {
            Piece* piece = game.getBoardTile(i, j);
            if (piece == nullptr) continue;
            shared_ptr<Texture> t = RessourceManager::getTexture(piece);
            if (t == nullptr) return;
            Sprite s(*t);
            s.setScale(SPRITE_SCALE, SPRITE_SCALE);
            s.setPosition(getWindowXPos(isFlipped? (7-i): i), getWindowYPos(isFlipped? (7-j): j));
            window.draw(s);
        }
    }
}

void GameThread::drawDraggedPiece(Piece* selectedPiece, RenderWindow& window, coor2d& mousePos) {
    if (selectedPiece == nullptr) return; // Safety check
    shared_ptr<Texture> t = RessourceManager::getTexture(selectedPiece);
    shared_ptr<Texture> tBefore = RessourceManager::getTexture(selectedPiece);

    if (t == nullptr || tBefore == nullptr) return;
    Sprite s(*t), sBefore(*tBefore); 
    s.setScale(SPRITE_SCALE, SPRITE_SCALE);
    sBefore.setScale(SPRITE_SCALE, SPRITE_SCALE);
    s.setPosition(mousePos.first, mousePos.second);
    sBefore.setPosition(
        (isFlipped? 7-selectedPiece->getY(): selectedPiece->getY()) * CELL_SIZE, 
        (isFlipped? 7-selectedPiece->getX(): selectedPiece->getX()) * CELL_SIZE + MENUBAR_HEIGHT
    );
    s.setOrigin(SPRITE_SIZE/2, SPRITE_SIZE/2);
    sBefore.setColor({255, 255, 255, 100});

    window.draw(sBefore);
    window.draw(s);
}

void GameThread::drawCurrentArrow(RenderWindow& window, Arrow& arrow) {
    shared_ptr<Texture> t = RessourceManager::getTexture(arrow.getFilename());
    if (t == nullptr) return;
    Sprite s(*t);

    cout << arrow.getOrigin().first << "," << arrow.getOrigin().second << endl;
    s.setPosition(arrow.getOrigin().first, arrow.getOrigin().second);
    s.setOrigin(0, s.getLocalBounds().height / 2);
    s.rotate(arrow.getRotation());
    window.draw(s);
}

void GameThread::drawAllArrows(RenderWindow& window, vector<Arrow>& arrows) {
    for (auto& arrow: arrows) {
        shared_ptr<Texture> t = RessourceManager::getTexture(arrow.getFilename());
        if (t == nullptr) return;
        Sprite s(*t);

        s.setPosition(arrow.getOrigin().first, arrow.getOrigin().second);
        s.setOrigin(0, s.getLocalBounds().height / 2);
        s.rotate(arrow.getRotation());
        window.draw(s);
    }
}

void GameThread::setTransitioningPiece(Piece* p, int xTarget, int yTarget, PieceTransition& trans) {
    trans.setTransitioningPiece(p);
    coor2d destination = {xTarget, yTarget};
    coor2d currPos = {p->getY() * CELL_SIZE, p->getX() * CELL_SIZE};
    trans.setDestination(destination);
    trans.setCurrPos(currPos);
    trans.setIsTransitioning(true);
    trans.setIncrement();
}

void GameThread::drawTransitioningPiece(RenderWindow& window, PieceTransition& piece, Board& game) {
    piece.move();
    shared_ptr<Texture> t = RessourceManager::getTexture(piece.getPiece());   
    if (t == nullptr) return;
    Sprite s(*t);
    s.setScale(SPRITE_SCALE, SPRITE_SCALE);
    s.setPosition(piece.getCurrPos().first, piece.getCurrPos().second + MENUBAR_HEIGHT);
    window.draw(s);
    piece.setHasArrived(piece.pieceIsInBounds(), game);
}
