#pragma once

#include <SFML/Graphics.hpp>

inline constexpr float g_BOX_HORIZONTAL_SCALE = 1.25;
inline constexpr float g_BOX_VERTICAL_SCALE = 1.25;
inline constexpr float g_BOX_TEXT_HORIZONTAL_SHIFT = 1.05;

typedef std::pair<int, int> coor2d;

class MoveBox
{
public:
    MoveBox(coor2d&, std::string&);

    float getScaledWidth() const { return g_BOX_HORIZONTAL_SCALE * m_textBounds.width; }
    float getScaledHeight() const { return g_BOX_VERTICAL_SCALE * m_textsf.getCharacterSize(); }
    void setPosition(coor2d coord) { m_position = coord; }
    sf::RectangleShape getRectangle() const { return m_rectangle; }
    sf::FloatRect getTextBounds() const { return m_textBounds; }
    coor2d getPosition() const { return m_position; }
    std::string getText() const { return m_text; }
    sf::Text getTextsf() const { return m_textsf; }
    bool isHowered(const coor2d&) const;
    void setIsCurrentMove();
    void setIsSelected();
    void setDefault();
    void handleRectangle();
    void handleText();

private:
    bool m_isSelected = false;
    sf::RectangleShape m_rectangle;
    sf::FloatRect m_textBounds;
    coor2d m_position;
    std::string m_text;
    sf::Text m_textsf;
};
