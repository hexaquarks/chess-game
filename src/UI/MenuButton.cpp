#include "../../include/UI/MenuButton.hpp"
#include "../../include/Application/GameThread.hpp"
#include "../../include/Logic/MoveTreeManager.hpp"
#include "../../include/Utilities/SFDrawUtil.hpp"

enum class MenuButtonType
{
    UNFOLD_OPTIONS,
    RESET_BOARD,
    FLIP_BOARD        
};

namespace 
{
    const Color transitionColors[5] =
    {
        { 218, 224, 242 }, 
        { 218, 224, 145 }, 
        { 218, 224, 200 },
        { 218, 224, 45 }, 
        { 218, 224, 242 }
    };

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

MenuButton::MenuButton(const std::string& name_, size_t index_, const Callback& callback_)
: Button(callback_)
, m_buttonType(static_cast<MenuButtonType>(index_))
{
    auto font = RessourceManager::getFont("Arial.ttf");
    m_text.setString(name_);
    m_text.setFont(*font);
    m_text.setCharacterSize(14);

    handleRectangle(index_, m_rectangle);
    handleSprite(index_, m_sprite);
    handleText(index_, m_text);
    
}

void MenuButton::doColorTransition()
{
    if (m_transitionColorIndex < 5)
    {
        Color currentColor = transitionColors[m_transitionColorIndex];
        currentColor.a = 255 - m_transitionColorIndex * 50; // Adjust alpha value
        m_rectangle.setFillColor(currentColor);
        ++m_transitionColorIndex;
    }
    else
    {
        m_isColorTransitioning = false;
        m_transitionColorIndex = 0;
        m_rectangle.setFillColor({ 218, 224, 241, 255 }); // Reset color
    }
}

bool MenuButton::isBoardReset() const
{
    return m_buttonType == MenuButtonType::RESET_BOARD;
}

void rotateIcon(sf::Sprite& sprite_)
{
    sprite_.setRotation(sprite_.getRotation() + 180);
}
}