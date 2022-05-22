#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
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

using namespace sf;

class SidePanel; // Forward declaration
class MoveSelectionPanel; // Forward declaration

// Constants
constexpr uint32_t g_MENUBAR_HEIGHT = 30;
constexpr uint32_t g_NUMBER_BUTTONS = 6;
constexpr uint32_t g_WINDOW_SIZE = 640;
constexpr uint32_t g_CELL_SIZE = g_WINDOW_SIZE / 8;
constexpr uint32_t g_BUTTON_POS = g_WINDOW_SIZE / g_NUMBER_BUTTONS;
constexpr uint32_t g_PANEL_SIZE = 640;
constexpr uint32_t g_BORDER_SIZE = 10;
constexpr uint32_t g_SOUTH_PANEL_HEIGHT = g_MENUBAR_HEIGHT;
constexpr uint32_t g_MAIN_PANEL_HEIGHT = g_PANEL_SIZE - g_SOUTH_PANEL_HEIGHT;
constexpr float g_SPRITE_SCALE = 0.6;
constexpr float g_SPRITE_SIZE = 128;
constexpr float g_BUTTON_SIZE = 40;

// Utility function
inline RectangleShape createSquare()
{
    return RectangleShape({g_CELL_SIZE, g_CELL_SIZE});
}

class GameThread
{
    inline static Board game;
    inline static RenderWindow window = {
        VideoMode(g_WINDOW_SIZE + g_PANEL_SIZE, g_WINDOW_SIZE + g_MENUBAR_HEIGHT),
        "Chess Game", Style::Titlebar | Style::Close
    };
    inline static vector<MenuButton> menuBar;
    inline static vector<Move> possibleMoves;
    inline static PieceTransition transitioningPiece;
    inline static MoveList moveList{game, transitioningPiece};
    inline static MoveTree moveTree;
    inline static MoveTree::Iterator treeIterator = moveTree.begin();
    inline static bool kingChecked = false;
    inline static bool noMoves = false;

    static void handleKeyPressed(const Event&, MoveSelectionPanel&, vector<Arrow>&, bool&);
    static void initializeBoard();
    static void initializeMenuBar();
    static void drawSidePanel(SidePanel&);
    static void drawCaptureCircles(const shared_ptr<Piece>&);
    static void highlightHoveredSquare(const shared_ptr<Piece>&, const coor2d&);
    static void drawMenuBar();
    static void drawPieces();
    static void drawDraggedPiece(const shared_ptr<Piece>&, const coor2d&);
    static void drawTransitioningPiece(PieceTransition&);
    static void drawAllArrows(vector<Arrow>&, const Arrow&);
    static void drawEndResults();
    static void drawKingCheckCircle();
    static void drawMoveSelectionPanel(int);
    static void drawGrayCover();

    inline static bool isFlipped = false;

    public:
    GameThread() = delete; // Delete constructor
    static void startGame();

    static int getTileXPos(const coor2d& pos_) { return pos_.first / g_CELL_SIZE; }
    static int getTileYPos(const coor2d& pos_)
    {
        if (pos_.second < g_MENUBAR_HEIGHT) return -1;
        return (pos_.second - g_MENUBAR_HEIGHT) / g_CELL_SIZE;
    }
    static int getWindowXPos(int i) { return i * g_CELL_SIZE; }
    static int getWindowYPos(int j) { return j * g_CELL_SIZE + g_MENUBAR_HEIGHT; }

    static void setTransitioningPiece(shared_ptr<Piece>&, int, int, int, int, PieceTransition&);
    static void setTransitioningPiece(shared_ptr<Piece>&, int, int, int, int, shared_ptr<Piece>&, int, int, PieceTransition&);

    static void flipBoard() { isFlipped = !isFlipped; }
    static bool boardFlipped() { return isFlipped; }
    static void refreshMoves() { possibleMoves = game.calculateAllMoves(); }
};
