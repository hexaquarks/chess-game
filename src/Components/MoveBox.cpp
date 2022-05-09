#include "../../include/Components/MoveBox.hpp"

MoveBox::MoveBox(coor2d position_, string text_): m_position(position_), m_text(text_) 
{
}

void MoveBox::handleText() 
{
    shared_ptr<Font> font =  RessourceManager::getFont("Arial.ttf");

    m_textsf.setString(m_text);
    m_textsf.setFont(*font);
    m_textsf.setCharacterSize(25);
    m_textsf.setStyle(Text::Bold);
    m_textsf.setFillColor(Color::Black);

    m_textBounds = m_textsf.getGlobalBounds();
}

void MoveBox::handleRectangle() 
{
    Vector2f recSize(m_textBounds.width, m_textsf.getCharacterSize());
    m_rectangle.setPosition(g_WINDOW_SIZE + m_position.first, g_MENUBAR_HEIGHT + m_position.second);
    m_rectangle.setSize(recSize);
    m_rectangle.setFillColor({ 50, 50, 50 });

    float positionalShift = ((g_BOX_HORIZONTAL_SCALE - 1.f)*m_rectangle.getLocalBounds().width)/2.f;

    m_rectangle.setScale(g_BOX_HORIZONTAL_SCALE, g_BOX_VERTICAL_SCALE);
    m_textsf.setPosition(g_WINDOW_SIZE + m_position.first + positionalShift,
                         g_MENUBAR_HEIGHT + m_position.second);
}

bool MoveBox::isHowered(coor2d& mousePos_) const 
{
    int x = mousePos_.first - g_WINDOW_SIZE;
    int y = mousePos_.second - g_MENUBAR_HEIGHT;

    return ((x > getPosition().first && x <= getPosition().first +  getScaledWidth()) 
         && (y > getPosition().second && y <= getPosition().second +  getScaledHeight()));
}

void MoveBox::setDefault() 
{
    m_textsf.setFillColor({ 240, 248, 255 }); // aliceblue
    m_rectangle.setFillColor({ 50, 50, 50 }); // charcoal
}

void MoveBox::setIsSelected()
{
    m_textsf.setFillColor({ 240, 248, 255 });  // charcoal
    m_rectangle.setFillColor({ 139, 148, 158 }); // aliceblue
}

void MoveBox::setIsCurrentMove() 
{
    m_textsf.setFillColor({ 50, 50, 50 });  // charcoal
    m_rectangle.setFillColor({ 240, 248, 255 }); // aliceblue
}

