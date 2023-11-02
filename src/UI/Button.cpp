#include "../../include/UI/Button.hpp"
#include "../../include/Application/GameThread.hpp"
#include "../../include/Logic/MoveTreeManager.hpp"
#include "../../include/Utilities/SFDrawUtil.hpp"
#include <SFML/Window/Event.hpp>

namespace 
{

    void handleRectangle(size_t i_, sf::RectangleShape& rectangle_)
    {
        SFDrawUtil::drawRectangleSf(rectangle_, ui::g_BUTTON_POS*i_, 0, { ui::g_BUTTON_POS, ui::g_MENUBAR_HEIGHT }, { 23,23,23 });
        rectangle_.setOutlineThickness(2.f);
        rectangle_.setOutlineColor({ 239, 242, 249 });
    }

    void handleSprite(size_t i_, sf::Sprite& sprite_)
    {
        sprite_.setOrigin(ui::g_BUTTON_SIZE/2, ui::g_BUTTON_SIZE/2);
        sprite_.setPosition(ui::g_BUTTON_POS*i_ + 20, ui::g_MENUBAR_HEIGHT/2);
        sprite_.setScale(ui::g_SPRITE_SCALE, ui::g_SPRITE_SCALE);
    }

    void handleText(size_t i, sf::Text& text_)
    {
        text_.setStyle(sf::Text::Bold);
        text_.setFillColor(sf::Color(240, 248, 255));
        text_.setOrigin(ui::g_BUTTON_SIZE/2 - ui::g_BUTTON_POS/3, ui::g_BUTTON_SIZE/1.75);
        text_.setPosition(ui::g_BUTTON_POS*i + ui::g_BUTTON_POS/3, ui::g_MENUBAR_HEIGHT);
    }
}


namespace ui
{   
    Button::Button(const std::string& name_, size_t index_, const Callback& callback_)
    : m_callback(callback_)
    {
        changeTexture(Normal);

        auto font = RessourceManager::getFont("Arial.ttf");
        m_text.setString(name_);
        m_text.setFont(*font);
        m_text.setCharacterSize(14);

        handleRectangle(index_, m_rectangle);
        handleSprite(index_, m_sprite);
        handleText(index_, m_text);

        //sf::FloatRect bounds = m_sprite.getLocalBounds();
        //m_text.setPosition(bounds.width / 2.f, bounds.height / 2.f);
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

    void Button::setSpriteTexture(sf::Texture& texture_)
    {
        m_sprite.setTexture(texture_);
    }

    void Button::draw(sf::RenderTarget& target_, sf::RenderStates states_) const
    {
        target_.draw(m_rectangle);
        target_.draw(m_sprite);
        target_.draw(m_text);
    }

    void Button::changeTexture(Type buttonType_)
    {
        // sf::IntRect textureRect(0, 50 * buttonType_, 200, 50);
        // m_sprite.setTextureRect(textureRect);
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

    void Button::rotateIcon()
    {
        m_sprite.setRotation(m_sprite.getRotation() + 180);
    }
}