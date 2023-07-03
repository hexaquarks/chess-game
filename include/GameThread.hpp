#pragma once

#include "UIManager.hpp"
#include "Ressources/RessourceManager.hpp"
#include "Components/MenuButton.hpp"
#include "Components/Board.hpp"
#include "Utilities/Arrow.hpp"
#include "Utilities/MoveTree.hpp"
#include "Utilities/MoveList.hpp"
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

class GameThread
{
public:
    GameThread() = delete; // Delete constructor
    static void startGame();

private:
    inline static Board m_board;

    inline static MoveList m_moveList{m_board};
    inline static MoveTree::Iterator& m_treeIterator = m_moveList.getIterator();
    inline static ui::UIManager m_uiManager{m_board, m_moveList};

    inline static bool m_isKingChecked = false;
    inline static bool m_noMovesAvailable = false;
    inline static shared_ptr<Piece> m_pLastMove;

    /* Graphical functions */
    static bool handleMouseButtonPressedLeft(Event&, ui::ClickState& clickState, ui::DragState&, ui::UIManager&);
    static bool handleMouseButtonPressedRight(Event&, ui::ClickState& clickState, ui::DragState&, ui::ArrowsInfo&);
    static bool handleMouseMoved(ui::ClickState&, ui::ArrowsInfo&, ui::UIManager&);
    static bool handleMouseButtonReleasedLeft(ui::ClickState&, ui::DragState&, ui::ArrowsInfo&, ui::UIManager&);
    static bool handleMouseButtonReleasedRight(ui::ClickState&, ui::DragState&, ui::ArrowsInfo&);
    static void handleKeyPressed(const Event&, ui::UIManager&, vector<Arrow>&);

    inline static bool m_isBoardFlipped = false;
};
