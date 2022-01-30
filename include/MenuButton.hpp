#pragma once
#include <SFML/Graphics.hpp>
#include "GameThread.hpp"
#include <string> 
using namespace sf;

class GameThread; // forward declaration to avoid cyclic dependency 

constexpr uint32_t BUTTON_POS = 640 / 6;

class MenuButton {
    int index;
    // Texture texture;
    Sprite sprite{};
    RectangleShape rectangle{};
    Text text{};
    std::string string;
    bool isRotatable;

    void handleRectangle(uint8_t);
    void handleSprite(uint8_t);
    void handleText(uint8_t);

    public: 
    MenuButton(uint8_t, bool, const std::string&);
    RectangleShape getRectangle() { return rectangle; }; 
    Sprite getSprite() { return sprite; };
    Text getText() { return text; };
    void setSpriteTexture(Texture& texture) { sprite.setTexture(texture); };
    void drawMenuButton(RenderWindow&); 
};