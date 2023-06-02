#include "../include/GameThread.hpp"
#include "../include/Components/MenuButton.hpp"
#include "../include/Utilities/PieceTransition.hpp"
#include "../include/Utilities/Move.hpp"
#include "../include/Ressources/AudioManager.hpp"
#include "../include/Components/SidePanel.hpp"
#include "../include/Components/MoveSelectionPanel.hpp"
#include "../include/Utilities/SFDrawUtil.hpp"
#include "./Ressources/Shader.cpp"
#include <iostream>
#include <vector>
#include <list>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <memory>

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

std::optional<Move> findSelectedMove(Board& board_, ui::ClickState& clickState_, int xPos_, int yPos_)
{
    for (auto& move : board_.getAllCurrentlyAvailableMoves())
    {
        if (move.getSelectedPiece() == clickState_.pSelectedPiece)
        {
            if (move.getTarget().first == yPos_ && move.getTarget().second == xPos_)
            {
                return move;
            }
        }
    }
    return std::nullopt;
}
} // anonymous namespace

void GameThread::startGame()
{
    ui::UIManager uiManager(board, moveList);

    // Parameters to handle a piece being dragged
    ui::DragState dragState;
    ui::ClickState clickState;
    ui::ArrowsInfo arrowsInfo;

    board.updateAllCurrentlyAvailableMoves();
    
    // Sounds for piece movement and capture
    AudioManager::getInstance().loadSound(SoundEffect::MOVE, "move.wav");
    AudioManager::getInstance().loadSound(SoundEffect::CAPTURE, "captures.wav");

    // This is the main loop (a.k.a game loop) this ensures that the program does not terminate until we exit
    Event event;
    while (uiManager.getWindow().isOpen())
    {
        uiManager.clearWindow();

        // We use a while loop for the pending events in case there were multiple events occured
        while (uiManager.getWindow().pollEvent(event))
        {
            if (event.type == Event::Closed) uiManager.getWindow().close();
            if (event.type == Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == Mouse::Left)
                {
                    if (!handleMouseButtonPressedLeft(event, clickState, dragState, uiManager)) continue;
                }
                if (event.mouseButton.button == Mouse::Right)
                {
                    if (!handleMouseButtonPressedRight(event, clickState, dragState, arrowsInfo)) continue;
                }
            }

            const bool isAPieceHandled = (dragState.pieceIsMoving || clickState.pieceIsClicked || clickState.isRightClicking);

            // Dragging a piece around
            if (event.type == Event::MouseMoved && isAPieceHandled)
            {
                if (!handleMouseMoved(clickState, arrowsInfo, uiManager)) continue;
            }

            // Mouse button released
            if (event.type == Event::MouseButtonReleased)
            {
                if (event.mouseButton.button == Mouse::Left)
                {    
                    if (!handleMouseButtonReleasedLeft(clickState, dragState, arrowsInfo, uiManager)) continue;
                }
                if (event.mouseButton.button == Mouse::Right)
                {
                    if (!handleMouseButtonReleasedRight(clickState, dragState, arrowsInfo)) continue;;
                }
            }

            if (event.type == Event::KeyPressed)
            {
                handleKeyPressed(event, uiManager.getMoveSelectionPanel(), arrowsInfo.arrows, uiManager.showMoveSelectionPanel());
            }
        }
        
        uiManager.draw(
            clickState, 
            dragState, 
            arrowsInfo, 
            kingChecked,  
            noMovesAvailable, 
            kingChecked);
        
        uiManager.display();
    }
}

bool GameThread::handleMouseButtonPressedLeft(Event& event, ui::ClickState& clickState, ui::DragState& dragState, ui::UIManager& uiManager)
{
    clickState.mousePos = {event.mouseButton.x, event.mouseButton.y};

    // Allow user to make moves only if they're at the current live position,
    // and if the click is on the chess board
    int yPos = ui::getTileYPos(clickState.mousePos);
    if (yPos < 0) return false;

    // Do not register click if Moveselection panel is activated
    // and the mouse is not within the panel's bounds
    if (uiManager.ignoreInputWhenSelectionPanelIsActive(clickState.mousePos)) return false;

    int xPos = ui::getTileXPos(clickState.mousePos, board.isFlipped());
    if (board.isFlipped()) yPos = 7 - yPos;
    auto& pPieceAtCurrentMousePos = board.getBoardTile(xPos, yPos);

    // If piece is not null and has the right color
    if (pPieceAtCurrentMousePos && pPieceAtCurrentMousePos->getTeam() == board.getTurn())
    {
        // Unselect clicked piece
        if (pPieceAtCurrentMousePos == clickState.pSelectedPiece)
        {
            clickState.pSelectedPiece.reset();
            clickState.pieceIsClicked = false;
            return false;
        }

        clickState.pSelectedPiece = pPieceAtCurrentMousePos;
        clickState.pieceIsClicked = false;

        dragState.pieceIsMoving = true;
        dragState.lastXPos = ui::getTileXPos(clickState.mousePos, board.isFlipped());
        dragState.lastYPos = yPos;

        // Set the tile on the board where the piece is selected to null
        board.resetBoardTile(dragState.lastXPos, dragState.lastYPos, false);
    }

    return true;
}

bool GameThread::handleMouseButtonPressedRight(Event& event, ui::ClickState& clickState, ui::DragState& dragState, ui::ArrowsInfo& arrowsInfo)
{
    if (!dragState.pieceIsMoving)
    {
        clickState.rightClickAnchor = {event.mouseButton.x, event.mouseButton.y};
        clickState.isRightClicking = true;

        arrowsInfo.currArrow.setOrigin(clickState.rightClickAnchor);
        arrowsInfo.currArrow.setDestination(clickState.rightClickAnchor);
    }

    return true;
}

bool GameThread::handleMouseMoved(ui::ClickState& clickState, ui::ArrowsInfo& arrowsInfo, ui::UIManager& uiManager)
{
    // Update the position of the piece that is being moved
    Vector2i mousePosition = Mouse::getPosition(uiManager.getWindow());
    clickState.mousePos = {mousePosition.x, mousePosition.y};

    if (clickState.isRightClicking)
    {
        arrowsInfo.currArrow.setDestination(clickState.mousePos);
        arrowsInfo.currArrow.updateArrow(); // Update the type and rotation
    }

    return true;
}

bool GameThread::handleMouseButtonReleasedLeft(ui::ClickState& clickState, ui::DragState& dragState, ui::ArrowsInfo& arrowsInfo, ui::UIManager& uiManager)
{
    // Handle menu bar buttons
    if (clickState.mousePos.second < ui::g_MENUBAR_HEIGHT)
    {
        for (auto& menuButton: uiManager.getMenuBar()) 
        {
            if (!menuButton.isMouseHovered(clickState.mousePos)) continue;
            menuButton.doMouseClick(board, moveList);
            if (!menuButton.isBoardReset()) continue;
            
            clickState.pSelectedPiece.reset();
            clickState.mousePos = {0, 0};

            arrowsInfo.arrows.clear();
            board.updateAllCurrentlyAvailableMoves();
        }
    }

    // Handle Side Panel Move Box buttons click
    uiManager.handleSidePanelMoveBox(clickState.mousePos);
    
    // ^^^ Possible bug here when moveboxe and moveselection panel overlap

    if (!clickState.pSelectedPiece) return false;

    // If clicked and mouse remained on the same square
    int xPos = ui::getTileXPos(clickState.mousePos, board.isFlipped());
    int yPos = ui::getTileYPos(clickState.mousePos, board.isFlipped());
    if (xPos == clickState.pSelectedPiece->getY() && yPos == clickState.pSelectedPiece->getX())
    {
        if (!clickState.pieceIsClicked)
        {
            // Put the piece back to it's square; it's not moving
            board.setBoardTile(dragState.lastXPos, dragState.lastYPos, clickState.pSelectedPiece, false);
            dragState.pieceIsMoving = false;
        }
        clickState.pieceIsClicked = !clickState.pieceIsClicked;
        return false;
    }

    // Try to match moves
    std::optional<Move> pSelectedMoveOpt = findSelectedMove(board, clickState, xPos, yPos);

    // If move is not allowed, place piece back, else apply the move
    if (!pSelectedMoveOpt.has_value())
    {
        board.setBoardTile(dragState.lastXPos, dragState.lastYPos, clickState.pSelectedPiece, false); // Cancel the move
    }
    else
    {
        MoveType type = pSelectedMoveOpt->getMoveType();
        auto pMove = make_shared<Move>(
            make_pair(xPos, yPos), 
            make_pair(dragState.lastXPos, dragState.lastYPos), 
            clickState.pSelectedPiece, 
            type);

        pMove->setCapturedPiece(pLastMove);
        pMove->setMoveArrows(arrowsInfo.arrows);
        moveList.addMove(pMove, arrowsInfo.arrows);

        pLastMove = clickState.pSelectedPiece;
        pLastMove->setLastMove(type);
        Piece::setLastMovedPiece(pLastMove);

        board.switchTurn();
        board.updateAllCurrentlyAvailableMoves();
        
        noMovesAvailable = board.getAllCurrentlyAvailableMoves().empty();
        if (noMovesAvailable) pMove->setNoMovesAvailable();

        if (board.kingIsChecked())
        {
            kingChecked = true;
            pMove->setChecked();
        }
        else
        {
            kingChecked = false;
            if (type == MoveType::CAPTURE || type == MoveType::ENPASSANT)
            {
                AudioManager::getInstance().playSound(SoundEffect::CAPTURE);
            } 
            else if (type == MoveType::NORMAL || type == MoveType::INIT_SPECIAL) 
            {
                AudioManager::getInstance().playSound(SoundEffect::MOVE);
            }
        }

        // moveTree.insertNode(pMove, treeIterator);
        // moveList.getMoves().printTree();
        arrowsInfo.arrows.clear();
    }

    clickState.pSelectedPiece.reset();
    clickState.pieceIsClicked = false;
    clickState.mousePos = {0, 0};
    dragState.pieceIsMoving = false;

    return true;
}

bool GameThread::handleMouseButtonReleasedRight(ui::ClickState& clickState, ui::DragState& dragState, ui::ArrowsInfo& arrowsInfo)
{ 
    if (clickState.pSelectedPiece && dragState.pieceIsMoving)
    {
        // Reset the piece back
        board.setBoardTile(dragState.lastXPos, dragState.lastYPos, clickState.pSelectedPiece, false);
        clickState.pSelectedPiece.reset();
        dragState.pieceIsMoving = false;
    }
    else if (clickState.isRightClicking)
    {
        // add arrow to arrow list to be drawn
        if (arrowsInfo.currArrow.isDrawable())
        {
            if (!arrowsInfo.currArrow.removeArrow(arrowsInfo.arrows)) 
            {
                arrowsInfo.arrows.push_back(arrowsInfo.currArrow);
            }
        }
        clickState.isRightClicking = false;
        clickState.rightClickAnchor = {0, 0};
        arrowsInfo.currArrow.resetParameters();
    }

    return true;
}

void GameThread::setTransitioningPiece(
    bool isUndo_, shared_ptr<Piece>& p_, int initialX_, int initialY_,
    int xTarget_, int yTarget_, shared_ptr<Piece>& captured_,
    int capturedXPos_, int capturedYPos_, PieceTransition& trans_
)
{
    trans_.resetPieces();
    trans_.setTransitioningPiece(p_);
    trans_.setDestination({ui::getWindowXPos(xTarget_), ui::getWindowYPos(yTarget_)});
    trans_.setCurrPos({ui::getWindowXPos(initialX_), ui::getWindowYPos(initialY_)});
    trans_.setCapturedPiece(captured_, ui::getWindowXPos(capturedXPos_), ui::getWindowYPos(capturedYPos_));
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
    trans_.setSecondDestination({ui::getWindowXPos(xTarget_), ui::getWindowYPos(yTarget_)});
    trans_.setSecondCurrPos({ui::getWindowXPos(initialX_), ui::getWindowYPos(initialY_)});
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
        transitioningPiece.setHasArrived();

    shared_ptr<Move> move;
    switch (event_.key.code)
    {
        case Keyboard::Left:
            moveList.goToPreviousMove(true, arrowList_);

            move = treeIterator.get()->m_move;
            checkIfMoveMakesKingChecked(move, kingChecked, noMovesAvailable);
            break;
        case Keyboard::Right:
            if (treeIterator.currentNodeHasMoreThanOneVariation())
            {
                if (showMoveSelectionPanel_)
                {
                    moveList.goToNextMove(true, moveSelectionPanel_.getSelection(), arrowList_);

                    move = treeIterator.get()->m_move;
                    checkIfMoveMakesKingChecked(move, kingChecked, noMovesAvailable);
                }
                showMoveSelectionPanel_ = !showMoveSelectionPanel_;
                return;
            }
            moveList.goToNextMove(true, std::nullopt, arrowList_);

            move = treeIterator.get()->m_move;
            checkIfMoveMakesKingChecked(move, kingChecked, noMovesAvailable);
            break;
        case Keyboard::LControl:
            board.flipBoard();
            break;
        case Keyboard::Up:
            showMoveSelectionPanel_
                ? moveSelectionPanel_.goToPreviousVariation()
                : moveList.goToCurrentMove(arrowList_);
            break;
        case Keyboard::Down:
            showMoveSelectionPanel_
                ? moveSelectionPanel_.goToNextVariation()
                : moveList.goToInitialMove(arrowList_);
            break;
        case Keyboard::Enter:
            if (showMoveSelectionPanel_)
            {
                moveList.goToNextMove(true, moveSelectionPanel_.getSelection(), arrowList_);
                
                move = treeIterator.get()->m_move;
                checkIfMoveMakesKingChecked(move, kingChecked, noMovesAvailable);

                showMoveSelectionPanel_ = false; // Close the panel display
            }
            break;
        default: break; // Avoid pattern matching not exhaustive warning
    }
}
