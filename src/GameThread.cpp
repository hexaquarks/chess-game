#include "../include/GameThread.hpp"
#include "../include/Utilities/MoveList.hpp"
#include "../include/Components/MenuButton.hpp"
#include "../include/Ressources/RessourceManager.hpp"
#include "../include/Utilities/PieceTransition.hpp"
#include "../include/Utilities/Move.hpp"
#include "../include/Components/SidePanel.hpp"
#include "../include/Components/MoveSelectionPanel.hpp"
#include "../include/Utilities/MoveTree.hpp"
#include "./Ressources/Shader.cpp"

#include <iostream>
#include <vector>
#include <list>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

using namespace sf;

void GameThread::startGame() {
    window.setFramerateLimit(60);

    // Load ressources 
    RessourceManager::loadRessources();

    // Setting window icon
    Image icon;
    icon.loadFromFile(RessourceManager::getIconPath("nw.png"));
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    window.setPosition({300, 300});

    // Parameters to handle a piece being dragged
    bool pieceIsMoving = false;
    bool pieceIsClicked = false;
    bool isRightClicking = false;
    Piece* selectedPiece = nullptr;
    Piece* movingPiece = nullptr; // Piece transition
    coor2d mousePos = {0, 0};
    coor2d rightClickAnchor = {0, 0};
    int lastXPos = 0, lastYPos = 0; // Last position of the piece before being dragged
    possibleMoves = game.calculateAllMoves();

    // Additional board state variables
    Piece* lastMove = nullptr;
    PieceTransition transitioningPiece;
    transitioningPiece.setTransitioningPiece();
    MoveList moveList(game, transitioningPiece);
    vector<Arrow> arrowList;
    Arrow arrow;
    MoveTree moveTree;
    moveTree.printTree();
    MoveTree::Iterator treeIterator = moveTree.begin();

    // Window parameters
    initializeMenuBar();
    SidePanel sidePanel{window, moveList};
    MoveSelectionPanel moveSelectionPanel{window, sidePanel};
    bool showMoveSelectionPanel = false;

    // Sounds for piece movement
    SoundBuffer bufferMove;
    if (!bufferMove.loadFromFile(RessourceManager::getAudioPath("move.wav"))) return;
    Sound soundMove;
    soundMove.setBuffer(bufferMove);

    SoundBuffer bufferCapture;
    if (!bufferCapture.loadFromFile(RessourceManager::getAudioPath("captures.wav"))) return;
    Sound soundCapture;
    soundCapture.setBuffer(bufferCapture);

    // This is the main loop (a.k.a game loop) this ensures that the program does not terminate until we exit
    Event event;
    while (window.isOpen()) {
        // window.clear(Color(218, 224, 241));
        window.clear(Color(23,23,23));

        // We use a while loop for the pending events in case there were multiple events occured
        while (window.pollEvent(event)) {

            // Closing the window
            if (event.type == Event::Closed)
                window.close();

            // Clicking on a piece
            if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Left) {
                    possibleMoves = game.calculateAllMoves();
                    // Get the tile of the click
                    mousePos = {event.mouseButton.x, event.mouseButton.y};

                    // Allow user to make moves only if they're at the current live position, 
                    // and if the click is on the chess board
                    int yPos = getTileYPos(mousePos);
                    // if (yPos < 0 || moveList.hasMovesAfter()) continue;
                    if (yPos < 0 ) continue; //testing

                    int xPos = isFlipped? 7-getTileXPos(mousePos): getTileXPos(mousePos);
                    if (isFlipped) yPos = 7-yPos;
                    Piece* piece = game.getBoardTile(xPos, yPos);

                    // If piece is not null and has the right color
                    if(game.getTurn() == Team::WHITE) {
                        cout << "white's turn " << endl;
                    } else { cout << "blacks's turn " << endl; }

                    if (piece != nullptr && piece->getTeam() == game.getTurn()) {
                        // Unselect clicked piece
                        if(piece == selectedPiece) {
                            selectedPiece = nullptr;
                            pieceIsClicked = false;
                            continue;
                        }

                        selectedPiece = piece;
                        pieceIsMoving = true;
                        pieceIsClicked = false;
                        lastXPos = isFlipped? 7-getTileXPos(mousePos): getTileXPos(mousePos); lastYPos = yPos;

                        // Set the tile on the board where the piece is selected to null
                        game.setBoardTile(lastXPos, lastYPos, nullptr, false); 
                    }
                }
                if (event.mouseButton.button == Mouse::Right) {
                    if (!pieceIsMoving) {
                        rightClickAnchor = {event.mouseButton.x, event.mouseButton.y};
                        arrow.setOrigin(rightClickAnchor);
                        arrow.setDestination(rightClickAnchor);
                        isRightClicking = true;
                    }
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
                    // Handle menu bar buttons
                    [[unlikely]] if (mousePos.second < MENUBAR_HEIGHT)
                        for (MenuButton& m: menuBar)
                            if (m.isClicked(mousePos))
                                if(m.performClick(game, moveList) == 1) {
                                    // TODO fix bug (reset at beggining)
                                    selectedPiece = nullptr;
                                    arrowList.clear();
                                    mousePos = {0, 0};
                                }
                    // Handle Side Panel Move Box buttons click
                    sidePanel.handleMoveBoxClicked(mousePos);

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
                        move.setMoveArrows(arrowList);
                        moveList.addMove(move, arrowList);
                        sidePanel.addMove(move);

                        moveTree.insertNode(move, treeIterator);
                        moveTree.printTree();
                        MoveTreeNode* temp = treeIterator.get();
                        // while(temp->m_parent != nullptr) temp = temp->m_parent;
                        moveTree.printPreorder(temp);

                        lastMove = selectedPiece;
                        lastMove->setLastMove(selectedMove->getMoveType());
                        Piece::setLastMovedPiece(lastMove);
                        game.switchTurn();
                        arrowList.clear();
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
                if (event.key.code == Keyboard::Left && !transitioningPiece.getIsTransitioning()) {
                    moveList.goToPreviousMove(true, arrowList);
                    moveTree.goToPreviousNode(treeIterator);
                }
                else if (event.key.code == Keyboard::Right && !transitioningPiece.getIsTransitioning())
                    moveList.goToNextMove(true, arrowList);
                else if (Keyboard::isKeyPressed(Keyboard::LControl) && Keyboard::isKeyPressed(Keyboard::F))
                    flipBoard();
                else if (event.key.code == Keyboard::Up) {
                    showMoveSelectionPanel
                        ? moveSelectionPanel.goToPreviousVariation(): moveList.goToCurrentMove(arrowList);
                }
                else if (event.key.code == Keyboard::Down) {
                    showMoveSelectionPanel
                        ? moveSelectionPanel.goToNextVariation(): moveList.goToInitialMove(arrowList);
                }
                else if (event.key.code == Keyboard::S) {
                    // testing
                    showMoveSelectionPanel = !showMoveSelectionPanel;
                }
            }
        }

        drawMenuBar();
        drawSidePanel(sidePanel);
        initializeBoard();
        moveList.highlightLastMove(window);
        if (game.kingIsChecked()) drawKingCheckCircle();

        if ((pieceIsMoving || pieceIsClicked) && selectedPiece != nullptr) {
            drawCaptureCircles(selectedPiece);
            highlightHoveredSquare(selectedPiece, mousePos);
        }
        drawPieces();
        if (pieceIsMoving) drawDraggedPiece(selectedPiece, mousePos);
        if (transitioningPiece.getIsTransitioning()) {
            drawTransitioningPiece(transitioningPiece);
        }
        drawAllArrows(arrowList, arrow);

        // End conditions
        if (possibleMoves.empty()) drawEndResults();
        if (showMoveSelectionPanel) {
            vector<string> testing{"1...e4", "1...d4", "1...c3", "1...c4"};
            moveSelectionPanel.drawMoveSelectionPanel(treeIterator);
        }

        window.display();
    }
}

void GameThread::initializeMenuBar() {
    constexpr uint16_t menuOptions = 3;
    const string menuNames[menuOptions] = {"Menu", "Reset", "Flip"};

    for (uint8_t i = 0; i < menuOptions; ++i) menuBar.push_back(MenuButton(i, menuNames[i]));
}

void GameThread::drawSidePanel(SidePanel& sidePanel) {
    // Draw the main panels
    RectangleShape mainPanel(Vector2f(PANEL_SIZE - 2*BORDER_SIZE, MAIN_PANEL_HEIGHT - 2*BORDER_SIZE));
    RectangleShape southPanel(Vector2f(PANEL_SIZE - 2*BORDER_SIZE, SOUTH_PANEL_HEIGHT));
    mainPanel.setFillColor(Color(50,50,50)); // cjarcoal
    southPanel.setFillColor(Color(50,50,50));
    mainPanel.setPosition(WINDOW_SIZE+BORDER_SIZE, MENUBAR_HEIGHT);
    southPanel.setPosition(WINDOW_SIZE+BORDER_SIZE, MENUBAR_HEIGHT+MAIN_PANEL_HEIGHT-BORDER_SIZE);
    
    window.draw(mainPanel);
    window.draw(southPanel);

    // Draw the content on the panels
    Vector2i position = sf::Mouse::getPosition(window);
    coor2d mousePos = {position.x , position.y};
    sidePanel.drawMoves(mousePos);
}

void GameThread::drawMenuBar() {
    constexpr uint16_t menuOptions = 3;
    const string iconFiles[menuOptions] = {"dropDownWhite.png", "resetWhite.png", "flipWhite.png"};

    for (uint8_t i = 0; i < menuOptions; ++i) {
        shared_ptr<Texture> t = RessourceManager::getTexture(iconFiles[i]);
        MenuButton& option = menuBar[i];
        option.setSpriteTexture(*t);

        // handle color transition of menu button when clicked
        if (option.getIsColorTransitioning()) option.doColorTransition();
        
        option.drawMenuButton(window);
    }   
}

void GameThread::initializeBoard() {
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

void GameThread::highlightHoveredSquare(Piece* selectedPiece, coor2d& mousePos) {
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

void GameThread::drawCaptureCircles(Piece* selectedPiece) {
    for (Move& move: possibleMoves) {
        int i = move.getTarget().second;
        int j = move.getTarget().first;
        if (move.getSelectedPiece() != selectedPiece) continue;
        bool isEmpty = game.getBoardTile(i, j) == nullptr;
        shared_ptr<Texture> t = RessourceManager::getTexture(isEmpty? "circle.png": "empty_circle.png");
        if (t == nullptr) return;
        Sprite circle(*t);
        if (isEmpty) circle.setScale(SPRITE_SCALE, SPRITE_SCALE);
        if (isFlipped) {i = 7-i; j = 7-j;}
        circle.setPosition(getWindowXPos(i), getWindowYPos(j));
        window.draw(circle);
    }
}

void GameThread::drawPieces() {
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

void GameThread::drawDraggedPiece(Piece* selectedPiece, coor2d& mousePos) {
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

void GameThread::drawAllArrows(vector<Arrow>& arrows, Arrow& currArrow) {
    if (arrows.empty()) return;
    arrows.emplace_back(currArrow);

    for (auto& arrow: arrows) {
        if(!arrow.isDrawable()) continue;

        shared_ptr<Texture> t = RessourceManager::getTexture(arrow.getFilename());
        if (t == nullptr) return;
        Sprite s(*t);
        coor2d arrowOrigin = arrow.getFormattedOrigin();

        if(arrow.isLArrow()) {
            s.setOrigin(CELL_SIZE/2 , s.getLocalBounds().height - CELL_SIZE/2);
            s.setPosition(arrowOrigin.first, arrowOrigin.second);
        }else {
            s.setOrigin(0, s.getLocalBounds().height / 2);
            s.setPosition(arrowOrigin.first, arrowOrigin.second);
        }
        s.rotate(arrow.getRotation());
        window.draw(s);
    }
    arrows.pop_back();
}

void GameThread::drawKingCheckCircle() {
    Shader shader;
    shader.loadFromMemory(VertexShader, RadialGradient);
    shader.setUniform("windowHeight", static_cast<float>(window.getSize().y)); 

    King* king = game.getKing();
    CircleShape c(CELL_SIZE/2);
    
    c.setFillColor(Color::Transparent);
    int x = isFlipped? 7-king->getY(): king->getY();
    int y = isFlipped? 7-king->getX(): king->getX();
    c.setPosition(getWindowXPos(x), getWindowYPos(y));
    shader.setUniform("color", Glsl::Vec4(1.f, 0.f, 0.f, 1.f));
    shader.setUniform("center", sf::Vector2f(c.getPosition().x + c.getRadius(), 
                      c.getPosition().y + + c.getRadius()));
    shader.setUniform("radius", c.getRadius());
    shader.setUniform("expand", 0.15f);

    window.draw(c, &shader);
}

void GameThread::drawEndResults() {
    // Checkmate
    if (game.kingIsChecked()) {
        King* losing = game.getKing();
        Texture t; t.loadFromFile(RessourceManager::getIconPath("checkmate.png"));
        Sprite checkmate(t);
        checkmate.setColor({255, 255, 255, 200});
        checkmate.setScale(0.5, 0.5);
        checkmate.setOrigin(40, 40);
        checkmate.setPosition(
            getWindowXPos(isFlipped? 7-losing->getY(): losing->getY())+CELL_SIZE,
            getWindowYPos(isFlipped? 7-losing->getX(): losing->getX())
        );
        window.draw(checkmate);
        return;
    }

    // Stalemate
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

void GameThread::drawTransitioningPiece(PieceTransition& piece) {
    piece.move();
    shared_ptr<Texture> t = RessourceManager::getTexture(piece.getPiece());   
    if (t == nullptr) return;
    Sprite s(*t);
    s.setScale(SPRITE_SCALE, SPRITE_SCALE);
    s.setPosition(piece.getCurrPos().first, piece.getCurrPos().second + MENUBAR_HEIGHT);
    window.draw(s);
    piece.setHasArrived(piece.pieceIsInBounds(), game);
}
