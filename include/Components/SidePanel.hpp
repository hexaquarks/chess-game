#pragma once

#include "../Utilities/Move.hpp"
#include "../Pieces/Piece.hpp"
#include "../Utilities/MoveList.hpp"
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

class SidePanel
{
public:
    SidePanel(RenderWindow&, MoveList&, bool&);

    pair<char, int> findLetterCoord(const coor2d&) const;
    string parseMove(const Move&, int, bool, bool = false) const;
    void resetNextPos() { m_nextPos = {ui::g_BORDER_SIZE + 10, 10}; }
    void goToNextRow(int height);
    void addMove(const Move&);
    void drawMoves(const coor2d&);
    void drawSquareBracket(coor2d&, int, bool) const;
    coor2d drawMove(const Move&, int, int, coor2d&, const coor2d&);
    void checkOutOfBounds(MoveBox&, int);
    void handleMoveBoxClicked(const coor2d&) const;
    void drawFromNode(const shared_ptr<MoveTreeNode>&, int, int, coor2d&, const coor2d&);

private:
    RenderWindow& m_window;
    MoveList& m_moveList;
    coor2d m_nextPos = {ui::g_BORDER_SIZE + 10, 10};
    int moveBoxCounter = 0;
    int m_row = 0;
    bool& m_showMoveSelectionPanel;

    string parseMoveHelper(const Move&, int, bool, bool) const;
};
