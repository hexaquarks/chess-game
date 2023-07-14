#include "../include/GameThread.hpp"
#include "../include/Components/MenuButton.hpp"
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

namespace game
{
    namespace
    {
        void playSoundsAfterNewMove(MoveType moveType_)
        {
            if (moveType_ == MoveType::CAPTURE || moveType_ == MoveType::ENPASSANT)
            {
                AudioManager::getInstance().playSound(SoundEffect::CAPTURE);
            } 
            else if (moveType_ == MoveType::NORMAL || moveType_ == MoveType::INIT_SPECIAL) 
            {
                AudioManager::getInstance().playSound(SoundEffect::MOVE);
            }
        }
    }

    void GameThread::startGame()
    {
        // Parameters to handle a piece being dragged
        ui::DragState dragState;
        ui::ClickState clickState;
        ui::ArrowsInfo arrowsInfo;

        m_board.updateAllCurrentlyAvailableMoves();
        
        // Sounds for piece movement and capture
        AudioManager::getInstance().loadSound(SoundEffect::MOVE, "move.wav");
        AudioManager::getInstance().loadSound(SoundEffect::CAPTURE, "captures.wav");

        // This is the main loop (a.k.a game loop) this ensures that the program does not terminate until we exit
        Event event;
        while (m_uiManager.getWindow().isOpen())
        {
            m_uiManager.clearWindow();

            // We use a while loop for the pending events in case there were multiple events occured
            while (m_uiManager.getWindow().pollEvent(event))
            {
                if (event.type == Event::Closed) m_uiManager.getWindow().close();
                if (event.type == Event::MouseButtonPressed)
                {
                    if (event.mouseButton.button == Mouse::Left)
                    {
                        if (!handleMouseButtonPressedLeft(event, clickState, dragState, m_uiManager)) continue;
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
                    if (!handleMouseMoved(clickState, arrowsInfo, m_uiManager)) continue;
                }

                // Mouse button released
                if (event.type == Event::MouseButtonReleased)
                {
                    if (event.mouseButton.button == Mouse::Left)
                    {    
                        if (!handleMouseButtonReleasedLeft(clickState, dragState, arrowsInfo, m_uiManager)) continue;
                    }
                    if (event.mouseButton.button == Mouse::Right)
                    {
                        if (!handleMouseButtonReleasedRight(clickState, dragState, arrowsInfo)) continue;;
                    }
                }

                if (event.type == Event::KeyPressed)
                {
                    handleKeyPressed(event, m_uiManager, arrowsInfo.arrows);
                }
            }
            
            m_uiManager.draw(
                clickState, 
                dragState, 
                arrowsInfo);
            
            m_uiManager.display();
        }
    }

    bool GameThread::handleMouseButtonPressedLeft(
        Event& event_, 
        ui::ClickState& clickState_, 
        ui::DragState& dragState_,
        ui::UIManager& uiManager_)
    {
        clickState_.mousePos = {event_.mouseButton.x, event_.mouseButton.y};

        // Allow user to make moves only if they're at the current live position,
        // and if the click is on the chess board
        int rank = ui::getRank(clickState_.mousePos, m_board.isFlipped());
        if (rank < 0) return false;

        // Do not register click if Moveselection panel is activated
        // and the mouse is not within the panel's bounds
        if (m_uiManager.ignoreInputWhenSelectionPanelIsActive(clickState_.mousePos)) return false;

        int file = ui::getFile(clickState_.mousePos, m_board.isFlipped());
        auto& pPieceAtCurrentMousePos = m_board.getBoardTile(file, rank);

        // If piece is not null and has the right color
        if (pPieceAtCurrentMousePos && pPieceAtCurrentMousePos->getTeam() == m_board.getTurn())
        {
            // Unselect clicked piece
            if (pPieceAtCurrentMousePos == clickState_.pSelectedPiece)
            {
                clickState_.pSelectedPiece.reset();
                clickState_.pieceIsClicked = false;
                return false;
            }

            clickState_.pSelectedPiece = pPieceAtCurrentMousePos;
            clickState_.pieceIsClicked = false;

            dragState_.pieceIsMoving = true;
            dragState_.lastFile = ui::getFile(clickState_.mousePos, m_board.isFlipped());
            dragState_.lastRank = rank;

            // Set the tile on the board where the piece is selected to null
            m_board.resetBoardTile(dragState_.lastFile, dragState_.lastRank, false);
        }

        return true;
    }

    bool GameThread::handleMouseButtonPressedRight(
        Event& event_, 
        ui::ClickState& clickState_, 
        ui::DragState& dragState_, 
        ui::ArrowsInfo& arrowsInfo_)
    {
        if (!dragState_.pieceIsMoving)
        {
            clickState_.rightClickAnchor = {event_.mouseButton.x, event_.mouseButton.y};
            clickState_.isRightClicking = true;

            arrowsInfo_.currArrow.setOrigin(clickState_.rightClickAnchor);
            arrowsInfo_.currArrow.setDestination(clickState_.rightClickAnchor);
        }

        return true;
    }

    bool GameThread::handleMouseMoved(
        ui::ClickState& clickState_, 
        ui::ArrowsInfo& arrowsInfo_, 
        ui::UIManager& uiManager_)
    {
        // Update the position of the piece that is being moved
        Vector2i mousePosition = Mouse::getPosition(uiManager_.getWindow());
        clickState_.mousePos = {mousePosition.x, mousePosition.y};

        if (clickState_.isRightClicking)
        {
            arrowsInfo_.currArrow.setDestination(clickState_.mousePos);
            arrowsInfo_.currArrow.updateArrow(); // Update the type and rotation
        }

        return true;
    }

    void GameThread::handleMouseButtonReleasedOnMenuBar(
        ui::ClickState& clickState_, 
        ui::DragState& dragState_, 
        ui::ArrowsInfo& arrowsInfo_, 
        ui::UIManager& uiManager_)
    {
        if (clickState_.mousePos.second >= ui::g_MENUBAR_HEIGHT) return;
        
        for (auto& menuButton: uiManager_.getMenuBar()) 
        {
            if (!menuButton.isMouseHovered(clickState_.mousePos)) continue;
            menuButton.doMouseClick(m_board, m_moveList);
            if (!menuButton.isBoardReset()) continue;
            
            clickState_.pSelectedPiece.reset();
            clickState_.mousePos = {0, 0};

            arrowsInfo_.arrows.clear();
            m_board.updateAllCurrentlyAvailableMoves();
        }
    }

    bool GameThread::handleMouseButtonReleasedLeft(
        ui::ClickState& clickState_, 
        ui::DragState& dragState_, 
        ui::ArrowsInfo& arrowsInfo_, 
        ui::UIManager& uiManager_)
    {
        handleMouseButtonReleasedOnMenuBar(clickState_, dragState_, arrowsInfo_, uiManager_);

        // Handle Side Panel Move Box buttons click
        uiManager_.handleSidePanelMoveBoxClick(clickState_.mousePos);
        
        // ^^^ Possible bug here when moveboxe and moveselection panel overlap

        if (!clickState_.pSelectedPiece) return false;

        // If clicked and mouse remained on the same square
        int rank = ui::getRank(clickState_.mousePos, m_board.isFlipped());
        int file = ui::getFile(clickState_.mousePos, m_board.isFlipped());
        if (rank == clickState_.pSelectedPiece->getRank() && file == clickState_.pSelectedPiece->getFile())
        {
            if (!clickState_.pieceIsClicked)
            {
                // Put the piece back to it's square; it's not moving
                m_board.setBoardTile(dragState_.lastFile, dragState_.lastRank, clickState_.pSelectedPiece, false);
                dragState_.pieceIsMoving = false;
            }
            clickState_.pieceIsClicked = !clickState_.pieceIsClicked;
            return false;
        }

        // Try to match moves
        std::optional<Move> pSelectedMoveOpt = m_board.findSelectedMove(clickState_.pSelectedPiece, rank, file);

        // If move is not allowed, place piece back, else apply the move
        if (!pSelectedMoveOpt.has_value())
        {
            m_board.setBoardTile(dragState_.lastFile, dragState_.lastRank, clickState_.pSelectedPiece, false); // Cancel the move
        }
        else
        {
            auto pMove = m_board.applyMoveOnBoard(
                pSelectedMoveOpt, 
                std::make_pair(file, rank), 
                std::make_pair(dragState_.lastFile, dragState_.lastRank),
                clickState_.pSelectedPiece, 
                arrowsInfo_.arrows);
            m_moveList.addMove(pMove, arrowsInfo_.arrows);
            m_board.updateBoardInfosAfterNewMove(clickState_.pSelectedPiece, pMove);
            arrowsInfo_.arrows.clear();

            playSoundsAfterNewMove(pMove->getMoveType());
        }

        m_uiManager.resetUserInputStatesAfterNewMove(clickState_, dragState_);

        return true;
    }

    bool GameThread::handleMouseButtonReleasedRight(
        ui::ClickState& clickState_, 
        ui::DragState& dragState_, 
        ui::ArrowsInfo& arrowsInfo_)
    { 
        if (clickState_.pSelectedPiece && dragState_.pieceIsMoving)
        {
            // Reset the piece back
            m_board.setBoardTile(dragState_.lastFile, dragState_.lastRank, clickState_.pSelectedPiece, false);
            clickState_.pSelectedPiece.reset();
            dragState_.pieceIsMoving = false;
        }
        else if (clickState_.isRightClicking)
        {
            // add arrow to arrow list to be drawn
            if (arrowsInfo_.currArrow.isDrawable())
            {
                if (!arrowsInfo_.currArrow.removeArrow(arrowsInfo_.arrows)) 
                {
                    arrowsInfo_.arrows.push_back(arrowsInfo_.currArrow);
                }
            }
            clickState_.isRightClicking = false;
            clickState_.rightClickAnchor = {0, 0};
            arrowsInfo_.currArrow.resetParameters();
        }

        return true;
    }

    void GameThread::handleKeyPressed(
        const Event& event_, 
        ui::UIManager& uiManager_,
        vector<Arrow>& arrowList_
    )
    {
        // If a piece is already moving, make it arrive
        if (m_moveList.isTransitionningPiece()) {
            m_moveList.setTransitioningPieceArrived();
        }

        shared_ptr<Move> move;
        MoveSelectionPanel& moveSelectionPanel = uiManager_.getMoveSelectionPanel();
        switch (event_.key.code)
        {
            case Keyboard::Left:
                m_moveList.goToPreviousMove(true, arrowList_);

                move = m_treeIterator.get()->m_move;
                m_board.checkIfMoveMakesKingChecked(move);
                break;
            case Keyboard::Right:
                if (m_treeIterator.currentNodeHasMoreThanOneVariation())
                {
                    if (!uiManager_.isMoveSelectionPanelOpen()) 
                    {
                        uiManager_.openMoveSelectionPanel();
                        return;
                    }

                    m_moveList.goToNextMove(true, moveSelectionPanel.getSelection(), arrowList_);

                    move = m_treeIterator.get()->m_move;
                    m_board.checkIfMoveMakesKingChecked(move);

                    uiManager_.closeMoveSelectionPanel();
                    return;
                }
                m_moveList.goToNextMove(true, std::nullopt, arrowList_);

                move = m_treeIterator.get()->m_move;
                m_board.checkIfMoveMakesKingChecked(move);
                break;
            case Keyboard::LControl:
                m_board.flipBoard();
                break;
            case Keyboard::Up:
                uiManager_.isMoveSelectionPanelOpen()
                    ? moveSelectionPanel.goToPreviousVariation()
                    : m_moveList.goToCurrentMove(arrowList_);
                break;
            case Keyboard::Down:
                uiManager_.isMoveSelectionPanelOpen()
                    ? moveSelectionPanel.goToNextVariation()
                    : m_moveList.goToInitialMove(arrowList_);
                break;
            case Keyboard::Enter:
                if (uiManager_.isMoveSelectionPanelOpen())
                {
                    m_moveList.goToNextMove(true, moveSelectionPanel.getSelection(), arrowList_);
                    
                    move = m_treeIterator.get()->m_move;
                    m_board.checkIfMoveMakesKingChecked(move);

                    uiManager_.closeMoveSelectionPanel();
                }
                break;
            default: break; // Avoid pattern matching not exhaustive warning
        }
    }
} // game namespace
