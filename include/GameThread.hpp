#pragma once

#include "UIManager.hpp"
#include "Ressources/RessourceManager.hpp"
#include "Components/MenuButton.hpp"
#include "Components/Board.hpp"
#include "Utilities/Arrow.hpp"
#include "Utilities/MoveTree.hpp"
#include "Utilities/MoveTreeManager.hpp"
#include "Pieces/Pawn.hpp"
#include "Pieces/Rook.hpp"
#include "Pieces/Knight.hpp"
#include "Pieces/Bishop.hpp"
#include "Pieces/King.hpp"
#include "Pieces/Queen.hpp"
#include "Pieces/Piece.hpp"

#include <SFML/Graphics.hpp>
#include <vector>

class SidePanel; 
class MoveSelectionPanel;

namespace game 
{
    class GameThread
    {
    public:
        void startGame();

    private:
        Board m_board;
        MoveTreeManager m_moveList{m_board};
        MoveTree::Iterator& m_treeIterator = m_moveList.getIterator();
        ui::UIManager m_uiManager{m_board, m_moveList};

        /* Event handles */
        bool handleMouseButtonPressedLeft(Event&, ui::ClickState& clickState, ui::DragState&, ui::UIManager&);
        bool handleMouseButtonPressedRight(Event&, ui::ClickState& clickState, ui::DragState&, ui::ArrowsInfo&);
        bool handleMouseMoved(ui::ClickState&, ui::ArrowsInfo&, ui::UIManager&);
        bool handleMouseButtonReleasedLeft(ui::ClickState&, ui::DragState&, ui::ArrowsInfo&, ui::UIManager&);
        bool handleMouseButtonReleasedRight(ui::ClickState&, ui::DragState&, ui::ArrowsInfo&);
        void handleKeyPressed(const Event&, ui::UIManager&, vector<Arrow>&);

        void handleMouseButtonReleasedOnMenuBar(ui::ClickState&, ui::DragState&, ui::ArrowsInfo&, ui::UIManager&);
    };
}
