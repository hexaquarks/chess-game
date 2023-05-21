#pragma once

#include "Ressources/RessourceManager.hpp"
#include "Utilities/PieceTransition.hpp"
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

class SidePanel;

namespace ui {
    constexpr int g_MENUBAR_HEIGHT = 30;
    constexpr int g_NUMBER_BUTTONS = 6;

    constexpr int g_WINDOW_SIZE = 640;
    constexpr float g_SPRITE_SCALE = 0.6;
    constexpr float g_SPECIAL_SCALE = 1.0;

    constexpr int g_CELL_SIZE = g_WINDOW_SIZE / 8;
    constexpr int g_BUTTON_POS = g_WINDOW_SIZE / g_NUMBER_BUTTONS;
    constexpr int g_PANEL_SIZE = 640;
    constexpr int g_BORDER_SIZE = 10;
    constexpr int g_SOUTH_PANEL_HEIGHT = g_MENUBAR_HEIGHT;
    constexpr int g_MAIN_PANEL_HEIGHT = g_PANEL_SIZE - g_SOUTH_PANEL_HEIGHT;
    constexpr float g_SPRITE_SIZE = 128;
    constexpr float g_BUTTON_SIZE = 40;

    inline sf::RectangleShape createSquare()
    {
        return sf::RectangleShape({g_CELL_SIZE, g_CELL_SIZE});
    }
    
    class UIManager {
        public:
            UIManager(sf::RenderWindow& window, Board& board);
            void draw();
            void drawMenuBar();
            void handleKeyPress();

            int getWindowXPos(int i) { return i * g_CELL_SIZE; }
            int getWindowYPos(int j) { return j * g_CELL_SIZE + g_MENUBAR_HEIGHT; }
            int getTileXPos(const coor2d& pos_) { return pos_.first / g_CELL_SIZE; }
            int getTileYPos(const coor2d& pos_)
            {
                if (pos_.second < g_MENUBAR_HEIGHT) return -1;
                return (pos_.second - g_MENUBAR_HEIGHT) / g_CELL_SIZE;
            }

            void drawSidePanel(SidePanel&);
            void drawCaptureCircles(const std::shared_ptr<Piece>&, vector<Move>&);
            void highlightHoveredSquare(const std::shared_ptr<Piece>&, const coor2d&, vector<Move>&);
            void drawPieces();
            void drawDraggedPiece(const std::shared_ptr<Piece>&, const coor2d&);
            void drawTransitioningPiece(PieceTransition&);
            void drawAllArrows(std::vector<Arrow>&, const Arrow&);
            void drawEndResults(bool);
            void drawKingCheckCircle();
            void drawMoveSelectionPanel(int);
            void drawGrayCover();
            void checkIfMoveMakesKingChecked(const std::shared_ptr<Move>&);
            void drawBoardSquares();

        private:
            sf::RenderWindow& m_window;
            Board& m_board;
            std::vector<MenuButton> m_menuBar;
            PieceTransition m_transitioningPiece;
            MoveList m_moveList{m_board, m_transitioningPiece};
            MoveTree::Iterator m_treeIterator = m_moveList.getIterator();
            bool m_isFlipped = false;

            void initializeMenuBar();

    }; 
}