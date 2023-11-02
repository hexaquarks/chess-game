#include "../../include/Application/GameThread.hpp"
#include "../../include/UI/Button.hpp"
#include "../../include/Logic/Move.hpp"
#include "../../include/Ressources/AudioManager.hpp"
#include "../../include/UI/SidePanel.hpp"
#include "../../include/UI/MoveSelectionPanel.hpp"
#include "../../include/Utilities/SFDrawUtil.hpp"
#include "../Ressources/Shader.cpp"

#include <iostream>
#include <cassert>
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

    // =================================================
    // Main game loop
    // =================================================
    void GameThread::startGame()
    {
        m_board.updateAllCurrentlyAvailableMoves();
        
        // Load all sounds
        AudioManager::getInstance().loadAllSounds();

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
                        if (!handleMouseButtonPressedLeft(event)) continue;
                    }
                    if (event.mouseButton.button == Mouse::Right)
                    {
                        if (!handleMouseButtonPressedRight(event)) continue;
                    }
                }

                const bool isAPieceHandled = (m_dragState.pieceIsMoving || m_clickState.pieceIsClicked || m_clickState.isRightClicking);

                // Dragging a piece around
                if (event.type == Event::MouseMoved && isAPieceHandled)
                {
                    if (!handleMouseMoved()) continue;
                }

                // Mouse button released
                if (event.type == Event::MouseButtonReleased)
                {
                    if (event.mouseButton.button == Mouse::Left)
                    {    
                        if (!handleMouseButtonReleasedLeft()) continue;
                    }
                    if (event.mouseButton.button == Mouse::Right)
                    {
                        if (!handleMouseButtonReleasedRight()) continue;;
                    }
                }

                if (event.type == Event::KeyPressed)
                {
                    handleKeyPressed(event, m_arrowsInfo.arrows);
                }
            }
            
            m_uiManager.draw();
            m_uiManager.display();
        }
    }

    // =================================================
    // Mouse button handles
    // =================================================
    bool GameThread::handleMouseButtonPressedLeft(Event& event_)
    {
        m_clickState.mousePos = {event_.mouseButton.x, event_.mouseButton.y};

        // Allow user to make moves only if they're at the current live position,
        // and if the click is on the chess board
        int rank = ui::getRank(m_clickState.mousePos, m_board.isFlipped());
        if (rank < 0 || m_clickState.mousePos.first >= ui::g_PANEL_SIZE) return false;

        // Do not register click if Moveselection panel is activated
        // and the mouse is not within the panel's bounds
        if (m_uiManager.ignoreInputWhenSelectionPanelIsActive(m_clickState.mousePos)) return false;

        int file = ui::getFile(m_clickState.mousePos, m_board.isFlipped());
        auto& pPieceAtCurrentMousePos = m_board.getBoardTile(file, rank);

        // If piece is not null and has the right color
        if (pPieceAtCurrentMousePos && pPieceAtCurrentMousePos->getTeam() == m_board.getTurn())
        {
            // Unselect clicked piece
            if (pPieceAtCurrentMousePos == m_clickState.pSelectedPiece)
            {
                m_clickState.pSelectedPiece.reset();
                m_clickState.pieceIsClicked = false;
                return false;
            }

            m_clickState.pSelectedPiece = pPieceAtCurrentMousePos;
            m_clickState.pieceIsClicked = false;

            m_dragState.pieceIsMoving = true;
            m_dragState.lastFile = ui::getFile(m_clickState.mousePos, m_board.isFlipped());
            m_dragState.lastRank = rank;

            // Set the tile on the board where the piece is selected to null
            m_board.resetBoardTile(m_dragState.lastFile, m_dragState.lastRank, false);
        }

        return true;
    }

    bool GameThread::handleMouseButtonPressedRight(Event& event_)
    {
        if (!m_dragState.pieceIsMoving)
        {
            m_clickState.rightClickAnchor = {event_.mouseButton.x, event_.mouseButton.y};
            m_clickState.isRightClicking = true;

            m_arrowsInfo.currArrow.setOrigin(m_clickState.rightClickAnchor);
            m_arrowsInfo.currArrow.setDestination(m_clickState.rightClickAnchor);
        }

        return true;
    }

    bool GameThread::handleMouseMoved()
    {
        // Update the position of the piece that is being moved
        Vector2i mousePosition = Mouse::getPosition(m_uiManager.getWindow());
        m_clickState.mousePos = {mousePosition.x, mousePosition.y};

        if (m_clickState.isRightClicking)
        {
            m_arrowsInfo.currArrow.setDestination(m_clickState.mousePos);
            m_arrowsInfo.currArrow.updateArrow(); // Update the type and rotation
        }

        return true;
    }

    void GameThread::handleMouseButtonReleasedOnMenuBar()
    {
        if (m_clickState.mousePos.second >= ui::g_MENUBAR_HEIGHT) return;
        
        for (auto& menuButton: m_uiManager.getMenuBar()) 
        {
            if (!menuButton.isMouseInBounds(m_clickState.mousePos)) continue;
            menuButton.doMouseClick();
        }
    }

    bool GameThread::handleMouseButtonReleasedLeft()
    {
        handleMouseButtonReleasedOnMenuBar();

        // Handle Side Panel Move Box buttons click
        m_uiManager.handleSidePanelMoveBoxClick(m_clickState.mousePos);
        
        // ^^^ Possible bug here when moveboxe and moveselection panel overlap

        if (!m_clickState.pSelectedPiece) return false;

        // If clicked and mouse remained on the same square
        int file = ui::getFile(m_clickState.mousePos, m_board.isFlipped());
        int rank = ui::getRank(m_clickState.mousePos, m_board.isFlipped());
        if (file == m_clickState.pSelectedPiece->getFile() && rank == m_clickState.pSelectedPiece->getRank())
        {
            if (!m_clickState.pieceIsClicked)
            {
                // Put the piece back to it's square; it's not moving
                m_board.setBoardTile(m_dragState.lastFile, m_dragState.lastRank, m_clickState.pSelectedPiece, false);
                m_dragState.pieceIsMoving = false;
            }
            m_clickState.pieceIsClicked = !m_clickState.pieceIsClicked;
            return false;
        }

        // Try to match moves
        std::optional<Move> pSelectedMoveOpt = m_board.findSelectedMove(m_clickState.pSelectedPiece, file, rank);

        // If move is not allowed, place piece back, else apply the move
        if (!pSelectedMoveOpt.has_value())
        {
            m_board.setBoardTile(m_dragState.lastFile, m_dragState.lastRank, m_clickState.pSelectedPiece, false); // Cancel the move
        }
        else
        {
            // auto pMove = m_board.applyMoveOnBoard(
            //     pSelectedMoveOpt.value().getMoveType(), 
            //     std::make_pair(file, rank), 
            //     std::make_pair(m_dragState.lastFile, m_dragState.lastRank),
            //     m_clickState.pSelectedPiece, 
            //     m_arrowsInfo.arrows);
            const auto pMove = std::make_shared<Move>(pSelectedMoveOpt.value());
            m_moveTreeManager.addMove(pMove, m_arrowsInfo.arrows);
            m_board.updateBoardInfosAfterNewMove(m_clickState.pSelectedPiece, pMove);
            m_arrowsInfo.arrows.clear();

            playSoundsAfterNewMove(pMove->getMoveType());
        }

        m_uiManager.resetUserInputStatesAfterNewMove(m_clickState, m_dragState);

        return true;
    }

    bool GameThread::handleMouseButtonReleasedRight()
    { 
        if (m_clickState.pSelectedPiece && m_dragState.pieceIsMoving)
        {
            // Reset the piece back
            m_board.setBoardTile(m_dragState.lastFile, m_dragState.lastRank, m_clickState.pSelectedPiece, false);
            m_clickState.pSelectedPiece.reset();
            m_dragState.pieceIsMoving = false;
        }
        else if (m_clickState.isRightClicking)
        {
            // add arrow to arrow list to be drawn
            if (m_arrowsInfo.currArrow.isDrawable())
            {
                if (!m_arrowsInfo.currArrow.removeArrow(m_arrowsInfo.arrows)) 
                {
                    m_arrowsInfo.arrows.push_back(m_arrowsInfo.currArrow);
                }
            }
            m_clickState.isRightClicking = false;
            m_clickState.rightClickAnchor = {0, 0};
            m_arrowsInfo.currArrow.resetParameters();
        }

        return true;
    }

    // =================================================
    // Key press handles
    // =================================================
    void GameThread::handleKeyPressLeft(vector<Arrow>& arrowList_) 
    {
        // Disable left navigation if the user is prompted to select a variation.
        if (m_uiManager.getMoveSelectionPanel().isOpen()) return;

        m_moveTreeManager.goToPreviousMove(true, arrowList_);
        auto prevMove = getCurrMoveTreeIteratorMove();
        m_board.checkIfMoveMakesKingChecked(prevMove);
    }

    void GameThread::handleKeyPressRight(vector<Arrow>& arrowList_) 
    {
        MoveSelectionPanel& moveSelectionPanel = m_uiManager.getMoveSelectionPanel();
        
        // Handle case with more than one variation
        if (m_treeIterator.currentNodeHasMoreThanOneVariation()) 
        {
            if (!moveSelectionPanel.isOpen()) 
            {
                moveSelectionPanel.open();
                return;
            }
            m_moveTreeManager.goToNextMove(true, moveSelectionPanel.getSelection(), arrowList_);

            auto nextMove = getCurrMoveTreeIteratorMove();
            m_board.checkIfMoveMakesKingChecked(nextMove);
            moveSelectionPanel.close();
            return;
        }
        m_moveTreeManager.goToNextMove(true, std::nullopt, arrowList_);
        auto nextMove = getCurrMoveTreeIteratorMove();
        m_board.checkIfMoveMakesKingChecked(nextMove);
    }

    void GameThread::handleKeyPressLControl() 
    {
        m_board.flipBoard();
    }

    void GameThread::handleKeyPressUp(vector<Arrow>& arrowList_) 
    {
        MoveSelectionPanel& moveSelectionPanel = m_uiManager.getMoveSelectionPanel();
        moveSelectionPanel.isOpen()
            ? moveSelectionPanel.goToPreviousVariation()
            : m_moveTreeManager.goToCurrentMove(arrowList_);
    }

    void GameThread::handleKeyPressDown(vector<Arrow>& arrowList_) 
    {
        MoveSelectionPanel& moveSelectionPanel = m_uiManager.getMoveSelectionPanel();
        moveSelectionPanel.isOpen()
            ? moveSelectionPanel.goToNextVariation()
            : m_moveTreeManager.goToInitialMove(arrowList_);
    }

    void GameThread::handleKeyPressEnter(vector<Arrow>& arrowList_) 
    {
        MoveSelectionPanel& moveSelectionPanel = m_uiManager.getMoveSelectionPanel();

        // Currently, enter is only relevent when the move selection
        // panel is up, in which enter executes the selected move.
        if (!moveSelectionPanel.isOpen()) return;
        
        m_moveTreeManager.goToNextMove(true, moveSelectionPanel.getSelection(), arrowList_);
        auto nextMove = getCurrMoveTreeIteratorMove();
        m_board.checkIfMoveMakesKingChecked(nextMove);
        moveSelectionPanel.close();
    }

    void GameThread::executeKeyHandler(
        const std::map<int, std::function<void()>>& keyMap_, 
        int keyCode_)
    {
        auto it = keyMap_.find(keyCode_);
        if (it != keyMap_.end()) it->second();
    }

    void GameThread::handleKeyPressed(
        const Event& event_,  
        vector<Arrow>& arrowList_)
    {
        if (m_moveTreeManager.isTransitionningPiece()) 
        {
            m_moveTreeManager.setTransitioningPieceArrived();
        }

        static std::map<int, std::function<void()>> keyMap = 
        {
            { Keyboard::Left, [this, &arrowList_] { handleKeyPressLeft(arrowList_); } },
            { Keyboard::Right, [this, &arrowList_] { handleKeyPressRight(arrowList_); } },
            { Keyboard::LControl, [this] { handleKeyPressLControl(); } },
            { Keyboard::Up, [this, &arrowList_] { handleKeyPressUp(arrowList_); } },
            { Keyboard::Down, [this, &arrowList_] { handleKeyPressDown(arrowList_); } },
            { Keyboard::Enter, [this, &arrowList_] { handleKeyPressEnter(arrowList_); } }
        };

        executeKeyHandler(keyMap, event_.key.code);
    }

    shared_ptr<Move> GameThread::getCurrMoveTreeIteratorMove() 
    {
        assert(m_treeIterator.get());
        return m_treeIterator.get()->m_move;
    }
} // game namespace
