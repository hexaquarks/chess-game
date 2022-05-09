#pragma once
#include <SFML/Graphics.hpp>
#include <string> 
using namespace sf;

typedef std::pair<int, int> coor2d;
class Board; // foward declaration
class MoveList; // foward declaration

inline const Color m_transitionColors[5] = 
{ 
    { 218, 224, 242 }, { 218, 224, 145 }, { 218, 224, 200 }, 
    { 218, 224, 45 }, { 218, 224, 242 } 
};

class MenuButton 
{
    int m_index;
    Sprite m_sprite;
    RectangleShape m_rectangle;
    Text m_text;
    std::string m_string;
    bool m_isRotatable;
    bool m_isColorTransitioning = false;
    int m_transitionColorIndex = 0;

    void handleRectangle(uint8_t);
    void handleSprite(uint8_t);
    void handleText(uint8_t);

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
    void rotateIcon(); 
    
    int performClick(Board&, MoveList&);
    bool isClicked(coor2d&) const; 
};
