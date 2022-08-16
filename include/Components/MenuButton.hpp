#pragma once

#include <SFML/Graphics.hpp>
#include <string>

using namespace sf;

typedef std::pair<int, int> coor2d;
class Board; // foward declaration
class MoveList; // foward declaration

class MenuButton
{
public:
    MenuButton(uint8_t, const std::string&, bool isRotatable = false);

    RectangleShape getRectangle() const { return m_rectangle; }
    Sprite getSprite() const { return m_sprite; }
    Text getText() const { return m_text; }

    int getTransitionColorIndex() const { return m_transitionColorIndex; }
    bool getIsColorTransitioning() const { return m_isColorTransitioning; }
    void setSpriteTexture(Texture& texture) { m_sprite.setTexture(texture); }
    
    void drawMenuButton(RenderWindow&) const;
    void doColorTransition();
    int performClick(Board&, MoveList&);
    bool isClicked(coor2d&) const;

private:
    int m_index;
    Sprite m_sprite;
    RectangleShape m_rectangle;
    Text m_text;
    std::string m_string;
    bool m_isRotatable;
    bool m_isColorTransitioning = false;
    int m_transitionColorIndex = 0;
};
