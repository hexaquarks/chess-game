#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

#include "RessourceManager.hpp"
#include "PieceTransition.hpp"
#include "MenuButton.hpp"
#include "Board.hpp"
#include "Arrow.hpp"

using namespace sf;

// Constants
const string WINDOW_TITLE = "Chess Game";
constexpr uint32_t WINDOW_SIZE = 640;
constexpr uint32_t CELL_SIZE = WINDOW_SIZE / 8;
constexpr uint32_t MENUBAR_HEIGHT = 30;
constexpr uint32_t NUMBER_BUTTONS = 6;
constexpr uint32_t BUTTON_POS = WINDOW_SIZE / NUMBER_BUTTONS;
constexpr float SPRITE_SCALE = 0.6;
constexpr float SPRITE_SIZE = 128;
constexpr float BUTTON_SIZE = 40;

// Utility function
inline RectangleShape createSquare() {
    return RectangleShape({CELL_SIZE, CELL_SIZE});
}

class GameThread {
    inline const static string iconsPath = "../assets/icons/";
    inline const static string audioPath = "../assets/sounds/";

    static void initializeBoard(RenderWindow&, Board&);
    static void initializeMenuBar(vector<MenuButton>&);
    static void drawCaptureCircles(RenderWindow&, vector<Move>&, Piece*, Board&);
    static void highlightHoveredSquare(RenderWindow&, Piece*, Board&, vector<Move>&, coor2d&);
    static void drawMenuBar(RenderWindow&, vector<MenuButton>&);
    static void drawPieces(RenderWindow&, Board&);
    static void drawDraggedPiece(Piece*, RenderWindow&, coor2d&);
    static void drawTransitioningPiece(RenderWindow&, PieceTransition&, Board&);
    static void drawAllArrows(RenderWindow&, vector<Arrow>&, Arrow&);

    inline static bool isFlipped = false;

    public:
    GameThread() = delete; // Delete constructor
    static void startGame();

    static int getTileXPos(coor2d& pos) { return pos.first / CELL_SIZE; }
    static int getTileYPos(coor2d& pos) {
        if (pos.second < MENUBAR_HEIGHT) return -1;
        return (pos.second - MENUBAR_HEIGHT) / CELL_SIZE;
    }
    static int getWindowXPos(int i) { return i * CELL_SIZE; }
    static int getWindowYPos(int j) { return j * CELL_SIZE + MENUBAR_HEIGHT; }

    static string getIconPath(const string& filename) { return iconsPath + filename; }
    static string getAudioPath(const string& filename) { return audioPath + filename; }
    static void setTransitioningPiece(Piece*, int,int, PieceTransition&);

    static void flipBoard() { isFlipped = !isFlipped; }
    static bool boardFlipped() { return isFlipped; }
};
