#pragma once

#include "../Utilities/UIConstants.hpp"

#include <SFML/Graphics.hpp>
#include <string>

typedef std::pair<int, int> coor2d;

// foward declarations
class Board; 
class MoveList; 
enum class MenuButtonType;

class MenuButton
{
public:
    MenuButton(const std::string&, size_t, bool isRotatable = false);
    MenuButton() = default;
    
    sf::RectangleShape getRectangle() const { return m_rectangle; }
    sf::Sprite getSprite() const { return m_sprite; }
    sf::Text getText() const { return m_text; }

    int getTransitionColorIndex() const { return m_transitionColorIndex; }
    bool getIsColorTransitioning() const { return m_isColorTransitioning; }

    void setSpriteTexture(sf::Texture& texture) { m_sprite.setTexture(texture); }
    
    void drawMenuButton(sf::RenderWindow&) const;
    void doColorTransition();
    void doMouseClick(Board&, MoveList&);
    bool isMouseHovered(coor2d&) const;
    bool isBoardReset() const;

private:
    MenuButtonType m_buttonType;
    sf::Sprite m_sprite;
    sf::RectangleShape m_rectangle;
    sf::Text m_text;
    std::string m_string;
    bool m_isRotatable;
    bool m_isColorTransitioning = false;
    int m_transitionColorIndex = 0;
};
