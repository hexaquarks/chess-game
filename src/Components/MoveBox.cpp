#include "../../include/Components/MoveBox.hpp"
#include "../../include/Utilities/SFDrawUtil.hpp"
#include "../../include/Utilities/UIConstants.hpp"
#include "../../include/Ressources/RessourceManager.hpp"

MoveBox::MoveBox(const coor2d& position_, const std::string& text_): m_position(position_), m_text(text_)
{
}

void MoveBox::handleText()
{
    auto font = RessourceManager::getFont("Arial.ttf");
    SFDrawUtil::drawTextSf(m_textsf, m_text, *font, 25, Text::Bold, Color::Black);

    m_textBounds = m_textsf.getGlobalBounds();
}

void MoveBox::handleRectangle()
{
    Vector2f recSize(m_textBounds.width, m_textsf.getCharacterSize());
    SFDrawUtil::drawRectangleSf(
        m_rectangle, ui::g_WINDOW_SIZE + m_position.first,
        ui::g_MENUBAR_HEIGHT + m_position.second, recSize, {50, 50, 50}
    );

    float positionalShift = ((g_BOX_HORIZONTAL_SCALE - 1.f)*m_rectangle.getLocalBounds().width) / 2.f;
    m_rectangle.setScale(g_BOX_HORIZONTAL_SCALE, g_BOX_VERTICAL_SCALE);
    m_textsf.setPosition(
        ui::g_WINDOW_SIZE + m_position.first + positionalShift,
        ui::g_MENUBAR_HEIGHT + m_position.second
    );
}

bool MoveBox::isHowered(const coor2d& mousePos_) const
{
    int x = mousePos_.first - ui::g_WINDOW_SIZE;
    int y = mousePos_.second - ui::g_MENUBAR_HEIGHT;

    return (
        (x > getPosition().first && x <= getPosition().first + getScaledWidth())
        && (y > getPosition().second && y <= getPosition().second + getScaledHeight())
    );
}

void MoveBox::setIsHovered(bool isMoveBoxHovered_)
{
    if (isMoveBoxHovered_)
    {
        // Selected color
        m_textsf.setFillColor({240, 248, 255}); // Charcoal
        m_rectangle.setFillColor({139, 148, 158}); // Aliceblue
    }
    else
    {
        // Default color
        m_textsf.setFillColor({240, 248, 255}); // Aliceblue
        m_rectangle.setFillColor({50, 50, 50}); // Charcoal
    }
}

void MoveBox::setIsCurrentMove()
{
    m_textsf.setFillColor({50, 50, 50}); // Charcoal
    m_rectangle.setFillColor({240, 248, 255}); // Aliceblue
}
