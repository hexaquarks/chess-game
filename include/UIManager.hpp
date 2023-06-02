#pragma once

#include "Ressources/RessourceManager.hpp"
#include "Components/MenuButton.hpp"
#include "Components/Board.hpp"
#include "Components/MoveSelectionPanel.hpp"
#include "Utilities/PieceTransition.hpp"
#include "Utilities/Arrow.hpp"
#include "Utilities/MoveTree.hpp"
#include "Pieces/Pawn.hpp"
#include "Pieces/Rook.hpp"
#include "Pieces/Knight.hpp"
#include "Pieces/Bishop.hpp"
#include "Pieces/King.hpp"
#include "Pieces/Queen.hpp"
#include "Pieces/Piece.hpp"
#include "Utilities/UIConstants.hpp"

#include <SFML/Graphics.hpp>

namespace ui {
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

    struct ArrowsInfo {
        std::vector<Arrow> arrows;
        Arrow currArrow;
    };

    inline sf::RectangleShape createSquare()
    {
        return sf::RectangleShape({g_CELL_SIZE, g_CELL_SIZE});
    }

    inline int getWindowXPos(int i) { return i * g_CELL_SIZE; }
    inline int getWindowYPos(int j) { return j * g_CELL_SIZE + g_MENUBAR_HEIGHT; }
    int getTileXPos(const coor2d& pos_, bool isFlipped_ = false);
    int getTileYPos(const coor2d& pos_, bool isFlipped_ = false);
    
    class UIManager {
        public:
            UIManager(Board&, MoveList&);
            void draw(ClickState&, DragState&, ArrowsInfo&, bool, bool, bool);

            void display() { m_window.display(); }
            void clearWindow() { m_window.clear({23, 23, 23}); }

            void handleSidePanelMoveBox(const coor2d&);
            bool ignoreInputWhenSelectionPanelIsActive(const coor2d&) const;
            bool& showMoveSelectionPanel() { return m_showMoveSelectionPanel; }

            // TODO architecture issue here. Should return a const ref ideally.
            MoveSelectionPanel& getMoveSelectionPanel() { return m_moveSelectionPanel; }
            std::vector<MenuButton>& getMenuBar() { return m_menuBar; }

            // A non-const ref is kind of necessary here. I want to delegate window
            // to this class, but I also want to keep window polling events
            // in GameThread. Good for now
            sf::RenderWindow& getWindow() { return m_window; }

        private:
            sf::RenderWindow m_window = {
                sf::VideoMode(g_WINDOW_SIZE + g_PANEL_SIZE, g_WINDOW_SIZE + g_MENUBAR_HEIGHT),
                "Chess Game", sf::Style::Titlebar | sf::Style::Close
            };
            Board& m_board;
            std::vector<MenuButton> m_menuBar;
            PieceTransition m_transitioningPiece;

            MoveList& m_moveList;
            SidePanel m_sidePanel;
            MoveSelectionPanel m_moveSelectionPanel;

            bool m_showMoveSelectionPanel = false;

            void initializeMenuBar();
            void drawMenuBar();
            void drawSidePanel(SidePanel&);
            void drawCaptureCircles(const std::shared_ptr<Piece>&, const vector<Move>&);
            void highlightHoveredSquare(const std::shared_ptr<Piece>&, const coor2d&, const vector<Move>&);
            void drawPieces();
            void drawDraggedPiece(const std::shared_ptr<Piece>&, const coor2d&);
            void drawTransitioningPiece(PieceTransition&);
            void drawAllArrows(std::vector<Arrow>&, const Arrow&);
            void drawEndResults(bool);
            void drawKingCheckCircle();
            void drawMoveSelectionPanel(int);
            void drawGrayCover();
            void drawBoardSquares();
            void highlightLastMove();
    }; 
}