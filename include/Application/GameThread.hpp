#pragma once

#include "../UI/UIManager.hpp"
#include "../Ressources/RessourceManager.hpp"
#include "../UI/MenuButton.hpp"
#include "../Logic/Board.hpp"
#include "../Utilities/Arrow.hpp"
#include "../Logic/MoveTree.hpp"
#include "../Logic/MoveTreeManager.hpp"
#include "../Logic/Pieces/Pawn.hpp"
#include "../Logic/Pieces/Rook.hpp"
#include "../Logic/Pieces/Knight.hpp"
#include "../Logic/Pieces/Bishop.hpp"
#include "../Logic/Pieces/King.hpp"
#include "../Logic/Pieces/Queen.hpp"
#include "../Logic/Pieces/Piece.hpp"

#include <SFML/Graphics.hpp>
#include <vector>

class SidePanel; 
class MoveSelectionPanel;

using KeyHandler = std::function<void()>;

namespace game 
{
    class GameThread
    {
    public:
        void startGame();

    private:
        Board m_board;
        MoveTreeManager m_moveTreeManager{m_board};
        MoveTree::Iterator& m_treeIterator = m_moveTreeManager.getIterator();
        ui::UIManager m_uiManager{m_board, m_moveTreeManager};

        shared_ptr<Move> getCurrMoveTreeIteratorMove();

        /* Event handles */
        bool handleMouseButtonPressedLeft(Event&, ui::ClickState& clickState, ui::DragState&, ui::UIManager&);
        bool handleMouseButtonPressedRight(Event&, ui::ClickState& clickState, ui::DragState&, ui::ArrowsInfo&);
        bool handleMouseMoved(ui::ClickState&, ui::ArrowsInfo&, ui::UIManager&);
        bool handleMouseButtonReleasedLeft(ui::ClickState&, ui::DragState&, ui::ArrowsInfo&, ui::UIManager&);
        bool handleMouseButtonReleasedRight(ui::ClickState&, ui::DragState&, ui::ArrowsInfo&);
        void handleKeyPressed(const Event&, ui::UIManager&, vector<Arrow>&);

        void handleMouseButtonReleasedOnMenuBar(ui::ClickState&, ui::DragState&, ui::ArrowsInfo&, ui::UIManager&);

        void handleKeyPressLeft(vector<Arrow>& arrowList_);
        void handleKeyPressRight(ui::UIManager& uiManager_, vector<Arrow>& arrowList_);
        void handleKeyPressLControl();  
        void handleKeyPressUp(ui::UIManager& uiManager_, vector<Arrow>& arrowList_);
        void handleKeyPressDown(ui::UIManager& uiManager_, vector<Arrow>& arrowList_);
        void handleKeyPressEnter(ui::UIManager& uiManager_, vector<Arrow>& arrowList_);

        void executeKeyHandler(const std::map<int, KeyHandler>& keyMap_, int keyCode_);

    };
}
