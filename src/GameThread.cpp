#include "../include/GameThread.hpp"
#include "../include/Components/MenuButton.hpp"
#include "../include/Ressources/RessourceManager.hpp"
#include "../include/Utilities/PieceTransition.hpp"
#include "../include/Utilities/Move.hpp"
#include "../include/Components/SidePanel.hpp"
#include "../include/Components/MoveSelectionPanel.hpp"
#include "../include/Utilities/DrawableSf.hpp"
#include "./Ressources/Shader.cpp"
#include "../include/UIManager.hpp"
#include <iostream>
#include <vector>
#include <list>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
using namespace sf;

void GameThread::checkIfMoveMakesKingChecked(const shared_ptr<Move>& move)
{
    if (move)
    {
        kingChecked = move->kingIsChecked();
        noMovesAvailable = move->hasNoMovesAvailable();
    }
    else
    {
        kingChecked = false;
        noMovesAvailable = false;
    }
}

void GameThread::startGame()
{
    ui::UIManager uiManager(window, game);

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
    bool showMoveSelectionPanel = false;
    SidePanel sidePanel(window, moveList, showMoveSelectionPanel);
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
                    {
                        for (auto& menuButton: menuBar) 
                        {
                            if (!menuButton.isMouseHovered(mousePos)) continue;
                            menuButton.doMouseClick(game, moveList);
                            if (!menuButton.isBoardReset()) continue;
                            
                            pSelectedPiece.reset();
                            arrowList.clear();
                            mousePos = {0, 0};
                            refreshMoves();
                        }
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
                        MoveType type = pSelectedMove->getMoveType();
                        shared_ptr<Move> pMove = make_shared<Move>(make_pair(xPos, yPos), make_pair(lastXPos, lastYPos), pSelectedPiece, type);

                        pMove->setCapturedPiece(pLastMove);
                        pMove->setMoveArrows(arrowList);
                        moveList.addMove(pMove, arrowList);

                        pLastMove = pSelectedPiece;
                        pLastMove->setLastMove(type);
                        Piece::setLastMovedPiece(pLastMove);

                        game.switchTurn();
                        refreshMoves();
                        
                        noMovesAvailable = possibleMoves.empty();
                        if (noMovesAvailable) pMove->setNoMovesAvailable();

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

                        // moveTree.insertNode(pMove, treeIterator);
                        // moveTree.printTree();
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
        
        uiManager.drawMenuBar();
        uiManager.drawBoardSquares();
        uiManager.drawSidePanel(sidePanel);
        moveList.highlightLastMove(window);
        if (kingChecked) uiManager.drawKingCheckCircle();

        if ((pieceIsMoving || pieceIsClicked) && pSelectedPiece)
        {
            uiManager.drawCaptureCircles(pSelectedPiece, possibleMoves);
            uiManager.highlightHoveredSquare(pSelectedPiece, mousePos, possibleMoves);
        }
        uiManager.drawPieces();
        if (pieceIsMoving) uiManager.drawDraggedPiece(pSelectedPiece, mousePos);
        if (transitioningPiece.getIsTransitioning()) {
            uiManager.drawTransitioningPiece(transitioningPiece);
        }
        uiManager.drawAllArrows(arrowList, arrow);

        // End conditions
        if (noMovesAvailable) uiManager.drawEndResults(kingChecked);

        if (showMoveSelectionPanel)
        {
            uiManager.drawGrayCover();
            moveSelectionPanel.drawMoveSelectionPanel(treeIterator);
        }

        window.display();
    }
}

void GameThread::setTransitioningPiece(
    bool isUndo_, shared_ptr<Piece>& p_, int initialX_, int initialY_,
    int xTarget_, int yTarget_, shared_ptr<Piece>& captured_,
    int capturedXPos_, int capturedYPos_, PieceTransition& trans_
)
{
    trans_.resetPieces();
    trans_.setTransitioningPiece(p_);
    trans_.setDestination({getWindowXPos(xTarget_), getWindowYPos(yTarget_)});
    trans_.setCurrPos({getWindowXPos(initialX_), getWindowYPos(initialY_)});
    trans_.setCapturedPiece(captured_, getWindowXPos(capturedXPos_), getWindowYPos(capturedYPos_));
    trans_.setUndo(isUndo_);
    trans_.setIsTransitioning(true);
    trans_.setIncrement();
}

void GameThread::setSecondTransitioningPiece(
    shared_ptr<Piece>& p_, int initialX_, int initialY_,
    int xTarget_, int yTarget_, PieceTransition& trans_
)
{
    trans_.setSecondTransitioningPiece(p_);
    trans_.setSecondDestination({getWindowXPos(xTarget_), getWindowYPos(yTarget_)});
    trans_.setSecondCurrPos({getWindowXPos(initialX_), getWindowYPos(initialY_)});
    trans_.setSecondIsTransitioning(true);
    trans_.setSecondIncrement();
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
            // moveTree.goToPreviousNode(treeIterator);
            move = treeIterator.get()->m_move;
            checkIfMoveMakesKingChecked(move);
            break;
        case Keyboard::Right:
            if (treeIterator.get()->childNumber > 1)
            {
                if (showMoveSelectionPanel_)
                {
                    moveList.goToNextMove(true, arrowList_);
                    // moveTree.goToNextNode(moveSelectionPanel_.getSelection(), treeIterator);
                    move = treeIterator.get()->m_move;
                    checkIfMoveMakesKingChecked(move);
                }
                showMoveSelectionPanel_ = !showMoveSelectionPanel_;
                return;
            }
            moveList.goToNextMove(true, arrowList_);
            // moveTree.goToNextNode(0, treeIterator);
            move = treeIterator.get()->m_move;
            checkIfMoveMakesKingChecked(move);
            break;
        case Keyboard::LControl:
            flipBoard();
            break;
        case Keyboard::Up:
            showMoveSelectionPanel_?
                moveSelectionPanel_.goToPreviousVariation(): moveList.goToCurrentMove(arrowList_);
            break;
        case Keyboard::Down:
            showMoveSelectionPanel_?
                moveSelectionPanel_.goToNextVariation(): moveList.goToInitialMove(arrowList_);
            break;
        case Keyboard::Enter:
            if (showMoveSelectionPanel_)
            {
                moveList.goToNextMove(true, arrowList_);
                // moveTree.goToNextNode(moveSelectionPanel_.getSelection(), treeIterator);
                kingChecked = treeIterator.get()->m_move->kingIsChecked();
                showMoveSelectionPanel_ = false; // Close the panel display
            }
            break;
        default: break; // Avoid pattern matching not exhaustive warning
    }
}
