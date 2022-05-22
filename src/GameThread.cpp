#include "../include/GameThread.hpp"
#include "../include/Components/MenuButton.hpp"
#include "../include/Ressources/RessourceManager.hpp"
#include "../include/Utilities/PieceTransition.hpp"
#include "../include/Utilities/Move.hpp"
#include "../include/Components/SidePanel.hpp"
#include "../include/Components/MoveSelectionPanel.hpp"
#include "../include/Utilities/DrawableSf.hpp"
#include "./Ressources/Shader.cpp"

#include <iostream>
#include <vector>
#include <list>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

using namespace sf;

void GameThread::startGame()
{
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
    shared_ptr<Piece> pSelectedPiece;
    shared_ptr<Piece> pMovingPiece; // Piece transition
    coor2d mousePos = {0, 0};
    coor2d rightClickAnchor = {0, 0};
    int lastXPos = 0;
    int lastYPos = 0; // Last position of the piece before being dragged
    possibleMoves = game.calculateAllMoves();

    // Additional board state variables
    shared_ptr<Piece> pLastMove;
    vector<Arrow> arrowList;
    Arrow arrow;

    // Window parameters
    initializeMenuBar();
    bool showMoveSelectionPanel = false;
    SidePanel sidePanel(window, moveList,moveTree, showMoveSelectionPanel);
    MoveSelectionPanel moveSelectionPanel(window, sidePanel);

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
    while (window.isOpen())
    {
        window.clear({23, 23, 23});

        // We use a while loop for the pending events in case there were multiple events occured
        while (window.pollEvent(event))
        {
            // Closing the window
            if (event.type == Event::Closed)
                window.close();

            // Clicking on a piece
            if (event.type == Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == Mouse::Left)
                {
                    // Get the tile of the click
                    mousePos = {event.mouseButton.x, event.mouseButton.y};

                    // Allow user to make moves only if they're at the current live position,
                    // and if the click is on the chess board
                    int yPos = getTileYPos(mousePos);
                    // if (yPos < 0 || moveList.hasMovesAfter()) continue;
                    if (yPos < 0) continue;

                    // Do not register click if Moveselection panel is activated
                    // and the mouse is not within the panel's bounds
                    if (showMoveSelectionPanel && !moveSelectionPanel.isHowered(mousePos)) continue;

                    int xPos = isFlipped? 7-getTileXPos(mousePos): getTileXPos(mousePos);
                    if (isFlipped) yPos = 7-yPos;
                    shared_ptr<Piece> pPiece = game.getBoardTile(xPos, yPos);

                    // If piece is not null and has the right color
                    if (pPiece && pPiece->getTeam() == game.getTurn())
                    {
                        // Unselect clicked piece
                        if (pPiece == pSelectedPiece)
                        {
                            pSelectedPiece.reset();
                            pieceIsClicked = false;
                            continue;
                        }

                        pSelectedPiece = pPiece;
                        pieceIsMoving = true;
                        pieceIsClicked = false;
                        lastXPos = isFlipped? 7-getTileXPos(mousePos): getTileXPos(mousePos); lastYPos = yPos;

                        // Set the tile on the board where the piece is selected to null
                        game.resetBoardTile(lastXPos, lastYPos, false);
                    }
                }
                if (event.mouseButton.button == Mouse::Right)
                {
                    if (!pieceIsMoving)
                    {
                        rightClickAnchor = {event.mouseButton.x, event.mouseButton.y};
                        arrow.setOrigin(rightClickAnchor);
                        arrow.setDestination(rightClickAnchor);
                        isRightClicking = true;
                    }
                }
            }

            // Dragging a piece around
            if (event.type == Event::MouseMoved && (pieceIsMoving || pieceIsClicked || isRightClicking))
            {
                // Update the position of the piece that is being moved
                Vector2i mousePosition = Mouse::getPosition(window);
                mousePos = {mousePosition.x, mousePosition.y};

                if (isRightClicking)
                {
                    arrow.setDestination(mousePos);
                    arrow.updateArrow(); // Update the type and rotation
                }
            }

            // Mouse button released
            if (event.type == Event::MouseButtonReleased)
            {
                if (event.mouseButton.button == Mouse::Left)
                {
                    // Handle menu bar buttons
                    if (mousePos.second < g_MENUBAR_HEIGHT)
                        for (MenuButton& m: menuBar)
                            if (m.isClicked(mousePos))
                                if (m.performClick(game, moveList) == 1)
                                {
                                    // TODO fix bug (reset at beggining)
                                    pSelectedPiece.reset();
                                    arrowList.clear();
                                    mousePos = {0, 0};
                                }
                    // Handle Side Panel Move Box buttons click
                    if (!showMoveSelectionPanel) sidePanel.handleMoveBoxClicked(mousePos);
                    // ^^^ Possible bug here when moveboxe and moveselection panel overlap

                    if (!pSelectedPiece) continue;

                    // If clicked and mouse remained on the same square
                    int xPos = isFlipped? 7-getTileXPos(mousePos): getTileXPos(mousePos);
                    int yPos = isFlipped? 7-getTileYPos(mousePos): getTileYPos(mousePos);
                    if (xPos == pSelectedPiece->getY() && yPos == pSelectedPiece->getX())
                    {
                        if (!pieceIsClicked)
                        {
                            // Put the piece back to it's square; it's not moving
                            game.setBoardTile(lastXPos, lastYPos, pSelectedPiece, false);
                            pieceIsMoving = false;
                        }
                        pieceIsClicked = !pieceIsClicked;
                        continue;
                    }

                    // Try to match moves
                    Move* pSelectedMove = nullptr;
                    for (auto& move: possibleMoves)
                    {
                        if (move.getSelectedPiece() == pSelectedPiece)
                        {
                            if (move.getTarget().first == yPos && move.getTarget().second == xPos)
                            {
                                pSelectedMove = &move;
                                break;
                            }
                        }
                    }

                    // If move is not allowed, place piece back, else apply the move
                    if (pSelectedMove == nullptr)
                    {
                        game.setBoardTile(lastXPos, lastYPos, pSelectedPiece, false); // Cancel the move
                    }
                    else
                    {
                        coor2d target = make_pair(xPos, yPos);
                        coor2d initial = make_pair(lastXPos, lastYPos);
                        MoveType type = pSelectedMove->getMoveType();

                        shared_ptr<Move> pMove = make_shared<Move>(target, initial, pSelectedPiece, type);

                        pMove->setCapturedPiece(pLastMove);
                        pMove->setMoveArrows(arrowList);
                        moveList.addMove(pMove, arrowList);

                        pLastMove = pSelectedPiece;
                        pLastMove->setLastMove(type);
                        Piece::setLastMovedPiece(pLastMove);

                        game.switchTurn();
                        refreshMoves();

                        noMoves = possibleMoves.empty();
                        if (noMoves) pMove->setNoMoves();

                        if (game.kingIsChecked())
                        {
                            kingChecked = true;
                            pMove->setChecked();
                        }
                        else
                        {
                            kingChecked = false;
                            if (type == MoveType::CAPTURE || type == MoveType::ENPASSANT) soundCapture.play();
                            else if (type == MoveType::NORMAL || type == MoveType::INIT_SPECIAL) soundMove.play();
                        }

                        moveTree.insertNode(pMove, treeIterator);
                        moveTree.printTree();
                        arrowList.clear();
                    }

                    pSelectedPiece.reset();
                    pieceIsMoving = false;
                    pieceIsClicked = false;
                    mousePos = {0, 0};
                }
                if (event.mouseButton.button == Mouse::Right)
                {
                    if (pSelectedPiece && pieceIsMoving)
                    {
                        // Reset the piece back
                        game.setBoardTile(lastXPos, lastYPos, pSelectedPiece, false);
                        pSelectedPiece.reset();
                        pieceIsMoving = false;
                    }
                    else if (isRightClicking)
                    {
                        // add arrow to arrow list to be drawn
                        if (arrow.isDrawable())
                        {
                            if (!arrow.removeArrow(arrowList)) arrowList.push_back(arrow);
                        }
                        isRightClicking = false;
                        rightClickAnchor = {0, 0};
                        arrow.resetParameters();
                    }
                }
            }

            if (event.type == Event::KeyPressed)
            {
                handleKeyPressed(event, moveSelectionPanel, arrowList, showMoveSelectionPanel);
            }
        }

        drawMenuBar();
        drawSidePanel(sidePanel);
        initializeBoard();
        moveList.highlightLastMove(window);
        if (kingChecked) drawKingCheckCircle();

        if ((pieceIsMoving || pieceIsClicked) && pSelectedPiece)
        {
            drawCaptureCircles(pSelectedPiece);
            highlightHoveredSquare(pSelectedPiece, mousePos);
        }
        drawPieces();
        if (pieceIsMoving) drawDraggedPiece(pSelectedPiece, mousePos);
        if (transitioningPiece.getIsTransitioning()) {
            drawTransitioningPiece(transitioningPiece);
        }
        drawAllArrows(arrowList, arrow);

        // End conditions
        if (noMoves) drawEndResults();

        if (showMoveSelectionPanel)
        {
            drawGrayCover();
            moveSelectionPanel.drawMoveSelectionPanel(treeIterator);
        }

        window.display();
    }
}

void GameThread::initializeMenuBar()
{
    constexpr uint16_t menuOptions = 3;
    const string menuNames[menuOptions] = {"Menu", "Reset", "Flip"};

    for (uint8_t i = 0; i < menuOptions; ++i) menuBar.push_back(MenuButton(i, menuNames[i]));
}

void GameThread::drawSidePanel(SidePanel& sidePanel_)
{
    // Draw the main panels
    RectangleShape mainPanel(Vector2f(g_PANEL_SIZE - 2*g_BORDER_SIZE, g_MAIN_PANEL_HEIGHT - 2*g_BORDER_SIZE));
    RectangleShape southPanel(Vector2f(g_PANEL_SIZE - 2*g_BORDER_SIZE, g_SOUTH_PANEL_HEIGHT));
    mainPanel.setFillColor({50, 50, 50}); // Charcoal
    southPanel.setFillColor({50, 50, 50});
    mainPanel.setPosition(g_WINDOW_SIZE + g_BORDER_SIZE, g_MENUBAR_HEIGHT);
    southPanel.setPosition(g_WINDOW_SIZE + g_BORDER_SIZE, g_MENUBAR_HEIGHT + g_MAIN_PANEL_HEIGHT - g_BORDER_SIZE);

    window.draw(mainPanel);
    window.draw(southPanel);

    // Draw the content on the panels
    Vector2i position = sf::Mouse::getPosition(window);
    coor2d mousePos = {position.x, position.y};
    sidePanel_.drawMoves(mousePos);
}

void GameThread::drawGrayCover()
{
    RectangleShape cover{};
    DrawableSf::drawRectangleSf(
        cover, 0, g_MENUBAR_HEIGHT,
        Vector2f(g_WINDOW_SIZE + g_PANEL_SIZE, g_WINDOW_SIZE), Color(220, 220, 220, 75)
    );
    window.draw(cover);
}

void GameThread::drawMenuBar()
{
    constexpr uint16_t menuOptions = 3;
    const string iconFiles[menuOptions] = {"dropDownWhite.png", "resetWhite.png", "flipWhite.png"};

    for (uint8_t i = 0; i < menuOptions; ++i)
    {
        shared_ptr<Texture> t = RessourceManager::getTexture(iconFiles[i]);
        MenuButton& option = menuBar[i];
        option.setSpriteTexture(*t);

        // Handle color transition of menu button when clicked
        if (option.getIsColorTransitioning()) option.doColorTransition();

        option.drawMenuButton(window);
    }
}

void GameThread::initializeBoard()
{
    const Color colours[2] = {{240, 217, 181}, {181, 136, 99}};

    for (uint8_t i = 0; i < 8; ++i)
    {
        for (uint8_t j = 0; j < 8; ++j)
        {
            // Drawing the colored square
            RectangleShape square = createSquare();
            DrawableSf::drawRectangleSf(square, getWindowXPos(i), getWindowYPos(j), square.getSize(), colours[(i+j)%2]);
            window.draw(square);
        }
    }
}

void GameThread::highlightHoveredSquare(const shared_ptr<Piece>& pSelectedPiece_, const coor2d& mousePos_)
{
    const Color colours[2] = {{173, 176, 134}, {100, 111, 64}};

    for (Move& move: possibleMoves)
    {
        int i = move.getTarget().second;
        int j = move.getTarget().first;
        if (isFlipped) {i = 7-i; j = 7-j;}
        if (move.getSelectedPiece() != pSelectedPiece_) continue;
        int xPos = getTileXPos(mousePos_), yPos = getTileYPos(mousePos_);

        if (i == xPos && j == yPos)
        {
            // Currently hovering a square where the piece can move
            RectangleShape square = createSquare();
            DrawableSf::drawRectangleSf(square, getWindowXPos(i), getWindowYPos(j), square.getSize(), colours[(i+j)%2]);
            window.draw(square);
        }
    }
}

void GameThread::drawCaptureCircles(const shared_ptr<Piece>& pSelectedPiece_)
{
    for (Move& move: possibleMoves)
    {
        int i = move.getTarget().second;
        int j = move.getTarget().first;

        if (move.getSelectedPiece() != pSelectedPiece_) continue;
        bool isEmpty = game.getBoardTile(i, j).get() == nullptr;
        const shared_ptr<Texture> t = RessourceManager::getTexture(isEmpty? "circle.png": "empty_circle.png");

        if (!t) return;
        Sprite circle(*t);
        if (isEmpty) circle.setScale(g_SPRITE_SCALE, g_SPRITE_SCALE);
        if (isFlipped) {i = 7-i; j = 7-j;}
        circle.setPosition(getWindowXPos(i), getWindowYPos(j));
        window.draw(circle);
    }
}

void GameThread::drawPieces()
{
    for (uint8_t i = 0; i < 8; ++i)
    {
        for (uint8_t j = 0; j < 8; ++j)
        {
            shared_ptr<Piece> piece = game.getBoardTile(i, j);
            if (!piece) continue;

            // Do not draw transitioning pieces
            if (transitioningPiece.getIsTransitioning()) {
                if (piece == transitioningPiece.getPiece() || piece == transitioningPiece.getCapturedPiece())
                    continue;
            }

            shared_ptr<Texture> t = RessourceManager::getTexture(piece);
            if (!t) return;

            Sprite s(*t);
            s.setScale(g_SPRITE_SCALE, g_SPRITE_SCALE);
            s.setPosition(getWindowXPos(isFlipped? (7-i): i), getWindowYPos(isFlipped? (7-j): j));
            window.draw(s);
        }
    }
}

void GameThread::drawDraggedPiece(const shared_ptr<Piece>& pSelectedPiece_, const coor2d& mousePos_)
{
    if (!pSelectedPiece_) return; // Safety check
    shared_ptr<Texture> t = RessourceManager::getTexture(pSelectedPiece_);
    shared_ptr<Texture> tBefore = RessourceManager::getTexture(pSelectedPiece_);

    if (!t || !tBefore) return;
    Sprite s(*t), sBefore(*tBefore);
    s.setScale(g_SPRITE_SCALE, g_SPRITE_SCALE);
    sBefore.setScale(g_SPRITE_SCALE, g_SPRITE_SCALE);
    s.setPosition(mousePos_.first, mousePos_.second);
    sBefore.setPosition(
        getWindowXPos(isFlipped? 7-pSelectedPiece_->getY(): pSelectedPiece_->getY()),
        getWindowYPos(isFlipped? 7-pSelectedPiece_->getX(): pSelectedPiece_->getX())
    );
    s.setOrigin(g_SPRITE_SIZE/2, g_SPRITE_SIZE/2);
    sBefore.setColor({255, 255, 255, 100});

    window.draw(sBefore);
    window.draw(s);
}

void GameThread::drawAllArrows(vector<Arrow>& arrows_, const Arrow& currArrow_)
{
    if (arrows_.empty()) return;
    arrows_.emplace_back(currArrow_);

    for (auto& arrow: arrows_)
    {
        if (!arrow.isDrawable()) continue;

        shared_ptr<Texture> t = RessourceManager::getTexture(arrow.getFilename());
        if (!t) return;
        Sprite s(*t);
        const coor2d arrowOrigin = arrow.getFormattedOrigin();

        if (arrow.isLArrow())
        {
            s.setOrigin(g_CELL_SIZE / 2, s.getLocalBounds().height - g_CELL_SIZE / 2);
            s.setPosition(arrowOrigin.first, arrowOrigin.second);
        }
        else
        {
            s.setOrigin(0, s.getLocalBounds().height / 2);
            s.setPosition(arrowOrigin.first, arrowOrigin.second);
        }
        s.rotate(arrow.getRotation());
        window.draw(s);
    }
    arrows_.pop_back();
}

void GameThread::drawKingCheckCircle()
{
    Shader shader;
    shader.loadFromMemory(VertexShader, RadialGradient);
    shader.setUniform("windowHeight", (float) window.getSize().y);

    shared_ptr<King> king = game.getKing();
    CircleShape c(g_CELL_SIZE / 2);
    c.setFillColor(Color::Transparent);

    int x = isFlipped? 7-king->getY(): king->getY();
    int y = isFlipped? 7-king->getX(): king->getX();
    c.setPosition(getWindowXPos(x), getWindowYPos(y));
    shader.setUniform("color", Glsl::Vec4(1.f, 0.f, 0.f, 1.f));
    shader.setUniform("center", Vector2f(
        c.getPosition().x + c.getRadius(), c.getPosition().y + + c.getRadius()
    ));
    shader.setUniform("radius", c.getRadius());
    shader.setUniform("expand", 0.15f);

    window.draw(c, &shader);
}

void GameThread::drawEndResults()
{
    // Checkmate
    if (kingChecked)
    {
        shared_ptr<King> losing = game.getKing();
        Texture t; t.loadFromFile(RessourceManager::getIconPath("checkmate.png"));
        Sprite checkmate(t);
        checkmate.setColor({255, 255, 255, 200});
        checkmate.setScale(0.5, 0.5);
        checkmate.setOrigin(40, 40);
        checkmate.setPosition(
            getWindowXPos(isFlipped? 7-losing->getY(): losing->getY()) + g_CELL_SIZE,
            getWindowYPos(isFlipped? 7-losing->getX(): losing->getX())
        );
        window.draw(checkmate);
        return;
    }
}

void GameThread::setTransitioningPiece(
    bool isUndo_, shared_ptr<Piece>& p_, int initialX_, int initialY_,
    int xTarget_, int yTarget_, shared_ptr<Piece>& captured_,
    int capturedXPos_, int capturedYPos_, PieceTransition& trans_
)
{
    trans_.setTransitioningPiece(p_);
    trans_.setDestination({getWindowXPos(xTarget_), getWindowYPos(yTarget_)});
    trans_.setCurrPos({getWindowXPos(initialX_), getWindowYPos(initialY_)});
    trans_.setCapturedPiece(captured_, getWindowXPos(capturedXPos_), getWindowYPos(capturedYPos_));
    trans_.setUndo(isUndo_);
    trans_.setIsTransitioning(true);
    trans_.setIncrement();
}

void GameThread::drawTransitioningPiece(PieceTransition& piece_)
{
    shared_ptr<Piece> captured = piece_.getCapturedPiece();
    piece_.move(game);
    shared_ptr<Texture> t = RessourceManager::getTexture(piece_.getPiece());
    if (!t) return;

    // Draw captured piece while transition is happening
    if (captured)
    {
        shared_ptr<Texture> t2 = RessourceManager::getTexture(captured);
        if (!t2) return;

        Sprite s2(*t2);
        s2.setScale(g_SPRITE_SCALE, g_SPRITE_SCALE);
        s2.setPosition(piece_.getCapturedX(), piece_.getCapturedY());

        Uint8 percentage = static_cast<Uint8>(piece_.getPercentageLeft() * 255);
        if (piece_.isUndo()) percentage = static_cast<Uint8>(255-percentage);
        s2.setColor({255, 255, 255, percentage});
        window.draw(s2);
    }

    Sprite s(*t);
    s.setScale(g_SPRITE_SCALE, g_SPRITE_SCALE);
    s.setPosition(piece_.getCurrPos().first, piece_.getCurrPos().second);
    window.draw(s);
}

void GameThread::handleKeyPressed(
    const Event& event_, MoveSelectionPanel& moveSelectionPanel_,
    vector<Arrow>& arrowList_, bool& showMoveSelectionPanel_
)
{
    // If a piece is already moving, make it arrive
    if (transitioningPiece.getIsTransitioning())
        transitioningPiece.setHasArrived(game);

    shared_ptr<Move> move;
    switch (event_.key.code)
    {
        case Keyboard::Left:
            moveList.goToPreviousMove(true, arrowList_);
            moveTree.goToPreviousNode(treeIterator);
            move = treeIterator.get()->m_move;
            if (move)
            {
                kingChecked = move->kingIsChecked();
                noMoves = move->hasNoMoves();
            }
            else
            {
                kingChecked = false;
                noMoves = false;
            }
            break;
        case Keyboard::Right:
            if (treeIterator.get()->childNumber > 1)
            {
                if (showMoveSelectionPanel_)
                {
                    moveList.goToNextMove(true, arrowList_);
                    moveTree.goToNextNode(moveSelectionPanel_.getSelection(), treeIterator);
                    move = treeIterator.get()->m_move;
                    if (move)
                    {
                        kingChecked = move->kingIsChecked();
                        noMoves = move->hasNoMoves();
                    }
                    else
                    {
                        kingChecked = false;
                        noMoves = false;
                    }
                }
                showMoveSelectionPanel_ = !showMoveSelectionPanel_;
                return;
            }
            moveList.goToNextMove(true, arrowList_);
            moveTree.goToNextNode(0, treeIterator);
            move = treeIterator.get()->m_move;
            if (move)
            {
                kingChecked = move->kingIsChecked();
                noMoves = move->hasNoMoves();
            }
            else
            {
                kingChecked = false;
                noMoves = false;
            }
            break;
        case Keyboard::LControl:
            flipBoard();
            break;
        case Keyboard::Up:
            showMoveSelectionPanel_
                ? moveSelectionPanel_.goToPreviousVariation(): moveList.goToCurrentMove(arrowList_);
            break;
        case Keyboard::Down:
            showMoveSelectionPanel_
                ? moveSelectionPanel_.goToNextVariation(): moveList.goToInitialMove(arrowList_);
            break;
        case Keyboard::Enter:
            if (showMoveSelectionPanel_)
            {
                moveList.goToNextMove(true, arrowList_);
                moveTree.goToNextNode(moveSelectionPanel_.getSelection(), treeIterator);
                kingChecked = treeIterator.get()->m_move->kingIsChecked();
                showMoveSelectionPanel_ = false; // Close the panel display
            }
            break;
        default: break; // Avoid pattern matching not exhaustive warning
    }
}
