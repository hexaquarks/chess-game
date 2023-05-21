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

namespace 
{
void checkIfMoveMakesKingChecked(
    const shared_ptr<Move>& move,
    bool& kingChecked_,
    bool& noMovesAvailable_)
{
    kingChecked_ = move ? move->kingIsChecked() : false;
    noMovesAvailable_ = move ? move->hasNoMovesAvailable() : false;
}

struct DragState {
    bool pieceIsMoving = false;
    
    // Last position of the piece before being dragged
    int lastXPos = 0;
    int lastYPos = 0; 
};

struct ClickState {
    coor2d mousePos = {0, 0};
    std::shared_ptr<Piece> pSelectedPiece;
    bool pieceIsClicked = false;
    bool isRightClicking = false;
    coor2d rightClickAnchor = {0, 0};
};

} // anonymous namespace

void GameThread::startGame()
{
    ui::UIManager uiManager(window, game);

    // Parameters to handle a piece being dragged
    DragState dragState;
    ClickState clickState;
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
            if (event.type == Event::Closed) window.close();
            if (event.type == Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == Mouse::Left)
                {
                    clickState.mousePos = {event.mouseButton.x, event.mouseButton.y};

                    // Allow user to make moves only if they're at the current live position,
                    // and if the click is on the chess board
                    int yPos = getTileYPos(clickState.mousePos);
                    if (yPos < 0) continue;

                    // Do not register click if Moveselection panel is activated
                    // and the mouse is not within the panel's bounds
                    if (showMoveSelectionPanel && !moveSelectionPanel.isHowered(clickState.mousePos)) continue;

                    int xPos = isFlipped? 7-getTileXPos(clickState.mousePos): getTileXPos(clickState.mousePos);
                    if (isFlipped) yPos = 7-yPos;
                    auto pPieceAtCurrentMousePos = game.getBoardTile(xPos, yPos);

                    // If piece is not null and has the right color
                    if (pPieceAtCurrentMousePos && pPieceAtCurrentMousePos->getTeam() == game.getTurn())
                    {
                        // Unselect clicked piece
                        if (pPieceAtCurrentMousePos == clickState.pSelectedPiece)
                        {
                            clickState.pSelectedPiece.reset();
                            clickState.pieceIsClicked = false;
                            continue;
                        }

                        clickState.pSelectedPiece = pPieceAtCurrentMousePos;
                        clickState.pieceIsClicked = false;

                        dragState.pieceIsMoving = true;
                        dragState.lastXPos = isFlipped? 7-getTileXPos(clickState.mousePos): getTileXPos(clickState.mousePos); 
                        dragState.lastYPos = yPos;

                        // Set the tile on the board where the piece is selected to null
                        game.resetBoardTile(dragState.lastXPos, dragState.lastYPos, false);
                    }
                }
                if (event.mouseButton.button == Mouse::Right)
                {
                    if (!dragState.pieceIsMoving)
                    {
                        clickState.rightClickAnchor = {event.mouseButton.x, event.mouseButton.y};
                        arrow.setOrigin(clickState.rightClickAnchor);
                        arrow.setDestination(clickState.rightClickAnchor);
                        clickState.isRightClicking = true;
                    }
                }
            }

            const bool aPieceIsHandled = (dragState.pieceIsMoving || clickState.pieceIsClicked || clickState.isRightClicking);

            // Dragging a piece around
            if (event.type == Event::MouseMoved && aPieceIsHandled)
            {
                // Update the position of the piece that is being moved
                Vector2i mousePosition = Mouse::getPosition(window);
                clickState.mousePos = {mousePosition.x, mousePosition.y};

                if (clickState.isRightClicking)
                {
                    arrow.setDestination(clickState.mousePos);
                    arrow.updateArrow(); // Update the type and rotation
                }
            }

            // Mouse button released
            if (event.type == Event::MouseButtonReleased)
            {
                if (event.mouseButton.button == Mouse::Left)
                {
                    // Handle menu bar buttons
                    if (clickState.mousePos.second < g_MENUBAR_HEIGHT)
                    {
                        for (auto& menuButton: menuBar) 
                        {
                            if (!menuButton.isMouseHovered(clickState.mousePos)) continue;
                            menuButton.doMouseClick(game, moveList);
                            if (!menuButton.isBoardReset()) continue;
                            
                            clickState.pSelectedPiece.reset();
                            clickState.mousePos = {0, 0};

                            arrowList.clear();
                            refreshMoves();
                        }
                    }

                    // Handle Side Panel Move Box buttons click
                    if (!showMoveSelectionPanel) sidePanel.handleMoveBoxClicked(clickState.mousePos);
                    // ^^^ Possible bug here when moveboxe and moveselection panel overlap

                    if (!clickState.pSelectedPiece) continue;

                    // If clicked and mouse remained on the same square
                    int xPos = isFlipped? 7-getTileXPos(clickState.mousePos): getTileXPos(clickState.mousePos);
                    int yPos = isFlipped? 7-getTileYPos(clickState.mousePos): getTileYPos(clickState.mousePos);
                    if (xPos == clickState.pSelectedPiece->getY() && yPos == clickState.pSelectedPiece->getX())
                    {
                        if (!clickState.pieceIsClicked)
                        {
                            // Put the piece back to it's square; it's not moving
                            game.setBoardTile(dragState.lastXPos, dragState.lastYPos, clickState.pSelectedPiece, false);
                            dragState.pieceIsMoving = false;
                        }
                        clickState.pieceIsClicked = !clickState.pieceIsClicked;
                        continue;
                    }

                    // Try to match moves
                    Move* pSelectedMove = nullptr;
                    for (auto& move: possibleMoves)
                    {
                        if (move.getSelectedPiece() == clickState.pSelectedPiece)
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
                        game.setBoardTile(dragState.lastXPos, dragState.lastYPos, clickState.pSelectedPiece, false); // Cancel the move
                    }
                    else
                    {
                        MoveType type = pSelectedMove->getMoveType();
                        shared_ptr<Move> pMove = make_shared<Move>(make_pair(xPos, yPos), make_pair(dragState.lastXPos, dragState.lastYPos), clickState.pSelectedPiece, type);

                        pMove->setCapturedPiece(pLastMove);
                        pMove->setMoveArrows(arrowList);
                        moveList.addMove(pMove, arrowList);

                        pLastMove = clickState.pSelectedPiece;
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

                    clickState.pSelectedPiece.reset();
                    clickState.pieceIsClicked = false;
                    clickState.mousePos = {0, 0};
                    dragState.pieceIsMoving = false;
                }
                if (event.mouseButton.button == Mouse::Right)
                {
                    if (clickState.pSelectedPiece && dragState.pieceIsMoving)
                    {
                        // Reset the piece back
                        game.setBoardTile(dragState.lastXPos, dragState.lastYPos, clickState.pSelectedPiece, false);
                        clickState.pSelectedPiece.reset();
                        dragState.pieceIsMoving = false;
                    }
                    else if (clickState.isRightClicking)
                    {
                        // add arrow to arrow list to be drawn
                        if (arrow.isDrawable())
                        {
                            if (!arrow.removeArrow(arrowList)) arrowList.push_back(arrow);
                        }
                        clickState.isRightClicking = false;
                        clickState.rightClickAnchor = {0, 0};
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

        if ((dragState.pieceIsMoving || clickState.pieceIsClicked) && clickState.pSelectedPiece)
        {
            uiManager.drawCaptureCircles(clickState.pSelectedPiece, possibleMoves);
            uiManager.highlightHoveredSquare(clickState.pSelectedPiece, clickState.mousePos, possibleMoves);
        }
        uiManager.drawPieces();
        if (dragState.pieceIsMoving) uiManager.drawDraggedPiece(clickState.pSelectedPiece, clickState.mousePos);
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
            checkIfMoveMakesKingChecked(move, kingChecked, noMovesAvailable);
            break;
        case Keyboard::Right:
            if (treeIterator.get()->childNumber > 1)
            {
                if (showMoveSelectionPanel_)
                {
                    moveList.goToNextMove(true, arrowList_);
                    // moveTree.goToNextNode(moveSelectionPanel_.getSelection(), treeIterator);
                    move = treeIterator.get()->m_move;
                    checkIfMoveMakesKingChecked(move, kingChecked, noMovesAvailable);
                }
                showMoveSelectionPanel_ = !showMoveSelectionPanel_;
                return;
            }
            moveList.goToNextMove(true, arrowList_);
            // moveTree.goToNextNode(0, treeIterator);
            move = treeIterator.get()->m_move;
            checkIfMoveMakesKingChecked(move, kingChecked, noMovesAvailable);
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
