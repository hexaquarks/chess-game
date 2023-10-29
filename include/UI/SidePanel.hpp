#pragma once

#include "../Logic/Move.hpp"
#include "../Logic/Pieces/Piece.hpp"
#include "../Logic/MoveTreeManager.hpp"
#include "MoveBox.hpp"
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

inline constexpr int g_INIT_WIDTH = ui::g_BORDER_SIZE + 10;
inline constexpr int g_INIT_HEIGHT = 10;
inline constexpr int g_ROW_HEIGHT = 35;
inline constexpr int g_HORIZONTAL_OFFSET = 30;
inline vector<MoveBox> moveBoxes;

class MoveTreeNode;
struct MoveInfo;

class SidePanel
{
public:
    SidePanel(RenderWindow&, MoveTreeManager&);
    SidePanel() = default;

    void resetNextPos() { m_nextPos = {ui::g_BORDER_SIZE + 10, 10}; }
    void goToNextRow(int height);
    void addMove(const MoveInfo&);
    void initializeMoveBoxCoodinates(const MoveInfo&, coor2d&);
    void drawMoves(std::vector<MoveInfo>, const coor2d&);
    void drawSquareBracket(coor2d&, int, bool) const;
    void drawMove(std::vector<MoveInfo>&, size_t, const coor2d&, bool);
    void checkOutOfBounds(MoveBox&, int);
    void handleMoveBoxClicked(const coor2d&) const;
    void handleMoveBoxOutOfBounds(coor2d&, MoveBox&, MoveInfo&);
    void drawFromNode(const shared_ptr<MoveTreeNode>&, int, int, coor2d&, const coor2d&);
    void drawMovePrefix(const std::string&, coor2d&);

private:
    RenderWindow& m_window;
    MoveTreeManager& m_moveTreeManager;
    coor2d m_nextPos = {ui::g_BORDER_SIZE + 10, 10};
    int moveBoxCounter = 0;
    int m_row = 0;
    int m_previousRow = 0;
};
