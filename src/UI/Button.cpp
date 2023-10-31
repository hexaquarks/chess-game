#include "../../include/UI/Button.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace UI
{
    Button::Button(Callback callBack_, sf::Sprite sprite_, sf::Text text_)
    : m_callback()
    , m_sprite(sprite_)
    , m_text(text_)
    , m_isToggle(false)
    {
        changeTexture(Normal);

        sf::FloatRect bounds = m_sprite.getLocalBounds();
        m_text.setPosition(bounds.width / 2.f, bounds.height / 2.f);
    }

    void Button::setCallback(Callback callback)
    {
        m_callback = std::move(callback);
    }

    void Button::setText(const std::string& text)
    {
        m_text.setString(text);
        // TODO : centerOrigin(m_text);
    }

    void Button::setToggle(bool flag)
    {
        m_isToggle = flag;
    }

    bool Button::isSelectable() const
    {
        return true;
    }

    void Button::select()
    {
        Component::select();

        changeTexture(Selected);
    }

    void Button::deselect()
    {
        Component::deselect();

        changeTexture(Normal);
    }

    void Button::activate()
    {
        Component::activate();

        // If we are toggle then we should show that the button is pressed and thus "toggled".
        if (m_isToggle) changeTexture(Pressed);

        if (m_callback) m_callback();

        // If we are not a toggle then deactivate the button since we are just momentarily activated.
        if (!m_isToggle) deactivate();
    }

    void Button::deactivate()
    {
        Component::deactivate();

        if (m_isToggle)
        {
            // Reset texture to right one depending on if we are selected or not.
            changeTexture(isSelected() ? Selected : Normal );
        }
    }

    void Button::draw(sf::RenderTarget& target_, sf::RenderStates states_) const
    {
        states_.transform *= getTransform();
        target_.draw(m_sprite, states_);
        target_.draw(m_text, states_);
    }

    void Button::changeTexture(Type buttonType_)
    {
        sf::IntRect textureRect(0, 50 * buttonType_, 200, 50);
        m_sprite.setTextureRect(textureRect);
    }

}