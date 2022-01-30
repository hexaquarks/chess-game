#include "../include/MenuButton.hpp"
#include "../include/GameThread.hpp"
#include <vector>
#include <string> 

Font font; 

MenuButton::MenuButton(uint8_t index, bool isRotatable,const std::string& name) : m_index(index) , m_isRotatable(isRotatable) {
    if (!font.loadFromFile("../assets/fonts/Arial.ttf")) return;

    m_text.setString(name);
    m_text.setFont(font);
    m_text.setCharacterSize(14);

    handleRectangle(index);
    handleSprite(index);
    handleText(index);
}

void MenuButton::handleRectangle(uint8_t i) {
    m_rectangle.setPosition(BUTTON_POS*i, 0);
    m_rectangle.setSize({BUTTON_POS, MENUBAR_HEIGHT});
    m_rectangle.setFillColor({218, 224, 241});
    m_rectangle.setOutlineThickness(2.f);
    m_rectangle.setOutlineColor({239, 242, 249});
}
void MenuButton::handleSprite(uint8_t i) {
    m_sprite.setOrigin(BUTTON_SIZE/2 + (BUTTON_POS / 2), BUTTON_SIZE/2);
    m_sprite.setPosition(BUTTON_POS*i + BUTTON_POS/2, MENUBAR_HEIGHT/2);
    m_sprite.setScale(SPRITE_SCALE, SPRITE_SCALE);
}
void MenuButton::handleText(uint8_t i) {
    m_text.setStyle(Text::Bold);
    m_text.setFillColor(Color::Black);
    m_text.setOrigin(BUTTON_SIZE/2 - (BUTTON_POS / 3), BUTTON_SIZE/(1.75));
    m_text.setPosition(BUTTON_POS*i + BUTTON_POS/3 , MENUBAR_HEIGHT);
}

void MenuButton::drawMenuButton(RenderWindow& window) {
    window.draw(m_rectangle);
    window.draw(m_sprite);
    window.draw(m_text);
}

