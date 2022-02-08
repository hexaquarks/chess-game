#pragma once
#include <SFML/Graphics.hpp>
#include "RessourceManager.hpp"
#include "Board.hpp"
#include "MenuButton.hpp"
#include <vector>
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
    inline static Piece* m_transitioningPiece = nullptr;
    static coor2d m_transitioningPieceDestination;

    static void initializeBoard(RenderWindow&, Board&);
    static void initializeMenuBar(vector<MenuButton>&);
    static void drawCaptureCircles(RenderWindow&, moveTypes&, Board&, RessourceManager&);
    static void highlightHoveredSquare(RenderWindow&, Board&, moveTypes&, coor2d&);
    static void drawMenuBar(RenderWindow&, vector<MenuButton>&, RessourceManager&);
    static void drawPieces(RenderWindow&, Board&, RessourceManager&);
    static void drawDraggedPiece(Piece*, RenderWindow&, coor2d&, RessourceManager&);
    static void removeIllegalMoves(Board&, moveTypes&, Piece*, coor2d&);
    static bool isPieceTransitioning() { return m_transitioningPiece != nullptr; }

    public:
    GameThread() = delete; // Delete constructor
    static void startGame();

    static void setTransitioningPiece(Piece* p, int xTarget, int yTarget);
    static void setTransitioningPieceDestination(coor2d& pos) {
        m_transitioningPieceDestination.first = pos.first; 
        m_transitioningPieceDestination.second = pos.second; 
    }
    static int getTileXPos(coor2d& pos) { return pos.first / CELL_SIZE; }
    static int getTileYPos(coor2d& pos) {
        if (pos.second < MENUBAR_HEIGHT) return -1;
        return (pos.second - MENUBAR_HEIGHT) / CELL_SIZE;
    }
    static int getWindowXPos(int i) { return i * CELL_SIZE; }
    static int getWindowYPos(int j) { return j * CELL_SIZE + MENUBAR_HEIGHT; }

    static string getIconPath(const string& filename) { return iconsPath + filename; }
    static string getAudioPath(const string& filename) { return audioPath + filename; }
};
