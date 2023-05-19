#pragma once

#include <SFML/Graphics.hpp>
#include <string>

using namespace sf;

typedef std::pair<int, int> coor2d;

// foward declarations
class Board; 
class MoveList; 
enum class MenuButtonType;

class MenuButton
{
public:
    MenuButton(const std::string&, size_t, bool isRotatable = false);

    RectangleShape getRectangle() const { return m_rectangle; }
    Sprite getSprite() const { return m_sprite; }
    Text getText() const { return m_text; }

    int getTransitionColorIndex() const { return m_transitionColorIndex; }
    bool getIsColorTransitioning() const { return m_isColorTransitioning; }
    void setSpriteTexture(Texture& texture) { m_sprite.setTexture(texture); }
    
    void drawMenuButton(RenderWindow&) const;
    void doColorTransition();
    void doMouseClick(Board&, MoveList&);
    bool isMouseHovered(coor2d&) const;
    bool isBoardReset() const;

private:
    MenuButtonType m_buttonType;
    Sprite m_sprite;
    RectangleShape m_rectangle;
    Text m_text;
    std::string m_string;
    bool m_isRotatable;
    bool m_isColorTransitioning = false;
    int m_transitionColorIndex = 0;
};
