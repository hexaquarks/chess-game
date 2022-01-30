#include "../include/MenuButton.hpp"
#include "../include/GameThread.hpp"

Font font;

MenuButton::MenuButton(uint8_t index, bool isRotatable, const std::string& name,const std::string& path) : index(index) , isRotatable(isRotatable) {
        if(!font.loadFromFile("../assets/fonts/Arial.ttf")) return;

        Texture text;
        text.loadFromFile(GameThread::getIconPath(path));
        Sprite s(texture);
        Text t(name,font,14);
        RectangleShape r;

        texture = text;
        rectangle = r;
        sprite = s;

        handleRectangle(index);
        handleSprite(index);
        handleText(index);
    }

void MenuButton::handleRectangle(uint8_t i) {
    rectangle.setPosition(BUTTON_POS*i, 0);
    rectangle.setSize({BUTTON_POS, MENUBAR_HEIGHT});
    rectangle.setFillColor({218, 224, 241});
    rectangle.setOutlineThickness(2.f);
    rectangle.setOutlineColor({239, 242, 249});
}
void MenuButton::handleSprite(uint8_t i) {
    sprite.setOrigin(BUTTON_SIZE/2 + (BUTTON_POS / 2), BUTTON_SIZE/2);
    sprite.setPosition(BUTTON_POS*i + BUTTON_POS/2, MENUBAR_HEIGHT/2);
    sprite.setScale(SPRITE_SCALE, SPRITE_SCALE);
}
void MenuButton::handleText(uint8_t i) {
    text.setStyle(Text::Bold);
    text.setFillColor(Color::Black);
    text.setOrigin(BUTTON_SIZE/2 - (BUTTON_POS / 3), BUTTON_SIZE/(1.75));
    text.setPosition(BUTTON_POS*i + BUTTON_POS/3 , MENUBAR_HEIGHT);
}

