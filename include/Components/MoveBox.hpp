#pragma once
#include "../GameThread.hpp"

using namespace std;
using namespace sf;

inline float BOX_HORIZONTAL_SCALE = 1.25;
inline float BOX_VERTICAL_SCALE = 1.25;
inline float BOX_TEXT_HORIZONTAL_SHIFT = 1.05;

class MoveBox {
    bool m_isSelected = false;
    RectangleShape m_rectangle;
    FloatRect m_textBounds;
    coor2d m_position;
    string m_text;
    Text m_textsf;

    public:
    MoveBox(coor2d, string);

    float getScaledWidth() { return BOX_HORIZONTAL_SCALE * m_textBounds.width; }
    float getScaledHeight() { return BOX_VERTICAL_SCALE * m_textsf.getCharacterSize(); }
    void setPosition(coor2d coord) { m_position = coord; }
    RectangleShape getRectangle() { return m_rectangle; }
    FloatRect getTextBounds() { return m_textBounds; }
    coor2d getPosition() { return m_position; }
    string getText() { return m_text; }
    Text getTextsf() { return m_textsf; }
    bool isHowered(coor2d&);
    void setIsCurrentMove();
    void setIsSelected();
    void setDefault();
    void handleRectangle();
    void handleText();
};