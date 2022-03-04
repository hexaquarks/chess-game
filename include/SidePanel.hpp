#pragma once

#include <SFML/Graphics.hpp>
#include "Move.hpp"
#include "Piece.hpp"
#include "GameThread.hpp"
#include "MoveList.hpp"
#include "MoveBox.hpp"

using namespace std;
using namespace sf;

inline const string letters = "abcdefgh";
inline vector<MoveBox> moveBoxes;

class SidePanel {
    RenderWindow& m_window;
    coor2d m_nextPos = {BORDER_SIZE + 10, 10};

    public:
    SidePanel(RenderWindow& window): m_window(window){};

    pair<char,int> findLetterCoord(coor2d);
    coor2d findNextAvailableSpot();
    string parseMove(Move&, int, bool);
    void resetNextPos() { m_nextPos = {BORDER_SIZE + 10, 10}; }
    void goToNextRow(int height);
    void addMove(MoveList&, Move&); 
    void drawMoves(coor2d&);
    void checkOutOfBounds(MoveBox&);
};