#pragma once

#include "../Ressources/RessourceManager.hpp"
#include "MenuButton.hpp"
#include "../Logic/Board.hpp"
#include "MoveSelectionPanel.hpp"
#include "../Utilities/PieceTransition.hpp"
#include "../Utilities/Arrow.hpp"
#include "../Logic/MoveTree.hpp"
#include "../Logic/Pieces/Pawn.hpp"
#include "../Logic/Pieces/Rook.hpp"
#include "../Logic/Pieces/Knight.hpp"
#include "../Logic/Pieces/Bishop.hpp"
#include "../Logic/Pieces/King.hpp"
#include "../Logic/Pieces/Queen.hpp"
#include "../Logic/Pieces/Piece.hpp"
#include "UIConstants.hpp"

#include <SFML/Graphics.hpp>

namespace ui {
    struct DragState {
        bool pieceIsMoving = false;

        // Last position of the piece before being dragged
        int lastFile = 0;
        int lastRank = 0; 
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

    inline int getWindowXPos(int file_) { return file_ * g_CELL_SIZE; }
    inline int getWindowYPos(int row_) { return row_ * g_CELL_SIZE + g_MENUBAR_HEIGHT; }
    int getFile(const coor2d& pos_, bool isFlipped_ = false);
    int getRank(const coor2d& pos_, bool isFlipped_ = false);
    
    class UIManager {
        public:
            UIManager(Board&, MoveTreeManager&);
            void draw(ClickState&, DragState&, ArrowsInfo&);

            void display() { m_window.display(); }
            void clearWindow() { m_window.clear({23, 23, 23}); }

            void handleSidePanelMoveBoxClick(const coor2d&);
            bool ignoreInputWhenSelectionPanelIsActive(const coor2d&) const;
        

            // TODO architecture issue here. Should return a const ref ideally.
            MoveSelectionPanel& getMoveSelectionPanel() { return m_moveSelectionPanel; }
            std::vector<MenuButton>& getMenuBar() { return m_menuBar; }

            // A non-const ref is kind of necessary here. I want to delegate window
            // to this class, but I also want to keep window polling events
            // in GameThread. Good for now
            sf::RenderWindow& getWindow() { return m_window; }

            void resetUserInputStatesAfterNewMove(ClickState&, DragState&);

        private:
            sf::RenderWindow m_window = {
                sf::VideoMode(g_WINDOW_SIZE + g_PANEL_SIZE, g_WINDOW_SIZE + g_MENUBAR_HEIGHT),
                "Chess Game", sf::Style::Titlebar | sf::Style::Close
            };
            Board& m_board;
            std::vector<MenuButton> m_menuBar;
            MoveTreeManager& m_moveTreeManager;
            SidePanel m_sidePanel;
            MoveSelectionPanel m_moveSelectionPanel;

            void initializeMenuBar();
            void drawMenuBar();
            void drawSidePanel();
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
            bool needToShowMoveSelectionPanel() const;
    }; 
}