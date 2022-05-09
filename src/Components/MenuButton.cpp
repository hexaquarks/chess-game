#include "../../include/Components/MenuButton.hpp"
#include "../../include/GameThread.hpp"
#include "../../include/Utilities/MoveList.hpp"

MenuButton::MenuButton(uint8_t index_, const std::string& name_, bool isRotatable_)
: m_index(index_), m_isRotatable(isRotatable_) 
{
    shared_ptr<Font> font =  RessourceManager::getFont("Arial.ttf");
    m_text.setString(name_);
    m_text.setFont(*font);
    m_text.setCharacterSize(14);

    handleRectangle(index_);
    handleSprite(index_);
    handleText(index_);
}

void MenuButton::handleRectangle(uint8_t i_) 
{
    m_rectangle.setPosition(g_BUTTON_POS*i_, 0);
    m_rectangle.setSize({ g_BUTTON_POS, g_MENUBAR_HEIGHT });
    m_rectangle.setFillColor({ 23,23,23 });
    m_rectangle.setOutlineThickness(2.f);
    m_rectangle.setOutlineColor({ 239, 242, 249 });
}

void MenuButton::handleSprite(uint8_t i_) 
{
    m_sprite.setOrigin(g_BUTTON_SIZE/2, g_BUTTON_SIZE/2);
    m_sprite.setPosition(g_BUTTON_POS*i_ + 20, g_MENUBAR_HEIGHT/2);
    m_sprite.setScale(g_SPRITE_SCALE, g_SPRITE_SCALE);
}

void MenuButton::handleText(uint8_t i) 
{
    m_text.setStyle(Text::Bold);
    m_text.setFillColor(Color(240, 248, 255));
    m_text.setOrigin(g_BUTTON_SIZE/2 - g_BUTTON_POS/3, g_BUTTON_SIZE/1.75);
    m_text.setPosition(g_BUTTON_POS*i + g_BUTTON_POS/3, g_MENUBAR_HEIGHT);
}

void MenuButton::rotateIcon() 
{
    m_sprite.setRotation(m_sprite.getRotation() + 180);
}

void MenuButton::drawMenuButton(RenderWindow& window_) const 
{
    window_.draw(m_rectangle);
    window_.draw(m_sprite);
    window_.draw(m_text);
}

bool MenuButton::isClicked(coor2d& mousePos_) const 
{
    float xi = m_rectangle.getGlobalBounds().left;
    float xf = xi + m_rectangle.getGlobalBounds().width;
    return mousePos_.first >= xi && mousePos_.first < xf;
}

void MenuButton::doColorTransition() 
{
    m_transitionColorIndex += 5;
    sf::Uint8 i = (sf::Uint8)m_transitionColorIndex;
    m_rectangle.setFillColor({ (sf::Uint8) (218-i), (sf::Uint8) (224-i), (sf::Uint8) (241-i) });

    if (m_transitionColorIndex == 45) 
    {
        m_isColorTransitioning = false;
        m_transitionColorIndex = 0;
        m_rectangle.setFillColor({ 218, 224, 241 });
    }
}

int MenuButton::performClick(Board& game_, MoveList& moveList_) 
{
    switch (m_index) 
    {
        case 0:
            // Clicked menu button
            rotateIcon();
            break;
        case 1: 
            // Clicked reset button
            game_.reset();
            moveList_.reset();

            break; 
        case 2: 
            // Clicked flip button
            GameThread::flipBoard();
            m_isColorTransitioning = true;
            break;
    }
    // make clicking shadow flashing animation
    m_isColorTransitioning = true;
    return m_index;
}
