#pragma once

#include "UIConstants.hpp"
#include "Button.hpp"

#include <SFML/Graphics.hpp>
#include <string>

// foward declarations
class Board; 
class MoveTreeManager; 
enum class MenuButtonType;

namespace ui
{
typedef std::function<void()> Callback;

class MenuButton : public Button
{
public:
    MenuButton(const std::string&, size_t, const Callback& callback_);
    MenuButton() = default;
    ~MenuButton() override = default;

    sf::RectangleShape getRectangle() const { return m_rectangle; }
    sf::Sprite getSprite() const { return m_sprite; }
    sf::Text getText() const { return m_text; }

    int getTransitionColorIndex() const { return m_transitionColorIndex; }
    bool getIsColorTransitioning() const { return m_isColorTransitioning; }

    void setSpriteTexture(sf::Texture& texture) { m_sprite.setTexture(texture); }
    
    void doColorTransition();
    bool isBoardReset() const;

private:
    MenuButtonType m_buttonType;
    bool m_isColorTransitioning = false;
    int m_transitionColorIndex = 0;
};

void rotateIcon(sf::Sprite& sprite_);
}

