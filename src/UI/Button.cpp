#include "../../include/UI/Button.hpp"

#include <SFML/Window/Event.hpp>

namespace ui
{
    Button::Button(const Callback& callback_) : m_callback(callback_)
    {
        changeTexture(Normal);

        sf::FloatRect bounds = m_sprite.getLocalBounds();
        m_text.setPosition(bounds.width / 2.f, bounds.height / 2.f);
    }

    void Button::setCallback(const Callback& callback_)
    {
        m_callback = std::move(callback_);
    }

    void Button::setText(const std::string& text)
    {
        m_text.setString(text);
        // TODO : centerOrigin(m_text);
    }

    void Button::draw(sf::RenderTarget& target_, sf::RenderStates states_) const
    {
        target_.draw(m_rectangle);
        target_.draw(m_sprite);
        target_.draw(m_text);
    }

    void Button::changeTexture(Type buttonType_)
    {
        sf::IntRect textureRect(0, 50 * buttonType_, 200, 50);
        m_sprite.setTextureRect(textureRect);
    }

    bool Button::isMouseInBounds(coor2d& mousePos_) const
    {
        float xi = m_rectangle.getGlobalBounds().left;
        float xf = xi + m_rectangle.getGlobalBounds().width;
        return mousePos_.first >= xi && mousePos_.first < xf;
    }

    void Button::doMouseClick() const
    {
        m_callback();
    }
}