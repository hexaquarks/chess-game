#pragma once

#include <SFML/Graphics.hpp>
#include "../Utilities/Move.hpp"
#include "../Pieces/Piece.hpp"
#include "../GameThread.hpp"
#include "../Utilities/MoveList.hpp"
#include "MoveBox.hpp"

using namespace std;
using namespace sf;

inline const string letters = "abcdefgh";
inline vector<MoveBox> moveBoxes;

class SidePanel {
    RenderWindow& m_window;
    MoveList& m_moveList;
    coor2d m_nextPos = {BORDER_SIZE + 10, 10};
    int moveBoxCounter = 0;

    public:
    SidePanel(RenderWindow& window, MoveList& moveList): m_window(window), m_moveList(moveList){};

    pair<char,int> findLetterCoord(coor2d) const;
    string parseMove(Move&, int, bool) const;
    void resetNextPos() { m_nextPos = {BORDER_SIZE + 10, 10}; }
    void goToNextRow(int height);
    void addMove(Move&); 
    void drawMoves(coor2d&) const;
    void checkOutOfBounds(MoveBox&);
    void handleMoveBoxClicked(coor2d&) const;
};