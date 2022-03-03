#pragma once

#include <SFML/Graphics.hpp>
#include "Move.hpp"
#include "Piece.hpp"
#include "GameThread.hpp"
#include "MoveList.hpp"

using namespace std;
using namespace sf;

inline const string letters = "abcdefgh";
inline vector<string> textBoxes;

class SidePanel {
    RenderWindow& m_window;
    int m_xPos = 5; // horizontal position for the next move text
    int m_yPos = 5; // vertical position for the next move text

    public:
    SidePanel(RenderWindow& window): m_window(window){};

    pair<char,int> findLetterCoord(coor2d);
    coor2d findNextAvailableSpot();
    string parseMove(Move&, int, bool);
    void getLocalCoordinates();
    void addMove(MoveList&, Move&); 
    void drawMoves();
};