#pragma once
#include "../GameThread.hpp"

using namespace std;
using namespace sf;

inline constexpr float g_BOX_HORIZONTAL_SCALE = 1.25;
inline constexpr float g_BOX_VERTICAL_SCALE = 1.25;
inline constexpr float g_BOX_TEXT_HORIZONTAL_SHIFT = 1.05;

class MoveBox
{
public:
    MoveBox(coor2d, string);

    float getScaledWidth() const { return g_BOX_HORIZONTAL_SCALE * m_textBounds.width; }
    float getScaledHeight() const { return g_BOX_VERTICAL_SCALE * m_textsf.getCharacterSize(); }
    void setPosition(coor2d coord) { m_position = coord; }
    RectangleShape getRectangle() const { return m_rectangle; }
    FloatRect getTextBounds() const { return m_textBounds; }
    coor2d getPosition() const { return m_position; }
    string getText() const { return m_text; }
    Text getTextsf() const { return m_textsf; }
    bool isHowered(coor2d&) const;
    void setIsCurrentMove();
    void setIsSelected();
    void setDefault();
    void handleRectangle();
    void handleText();

private:
    bool m_isSelected = false;
    RectangleShape m_rectangle;
    FloatRect m_textBounds;
    coor2d m_position;
    string m_text;
    Text m_textsf;

};