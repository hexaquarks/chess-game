#include "../../include/Components/MoveBox.hpp"

MoveBox::MoveBox(coor2d position, string text): m_position(position), m_text(text) {};

void MoveBox::handleText() {
    shared_ptr<Font> font =  RessourceManager::getFont("Arial.ttf");

    m_textsf.setString(m_text);
    m_textsf.setFont(*font);
    m_textsf.setCharacterSize(25);
    m_textsf.setStyle(Text::Bold);
    m_textsf.setFillColor(Color::Black);

    m_textBounds = m_textsf.getGlobalBounds();
}

void MoveBox::handleRectangle() {
    Vector2f recSize(m_textBounds.width, m_textsf.getCharacterSize());
    m_rectangle.setPosition(WINDOW_SIZE + m_position.first, MENUBAR_HEIGHT + m_position.second);
    m_rectangle.setSize(recSize);
    m_rectangle.setFillColor(Color(50,50,50));

    float positionalShift = ((BOX_HORIZONTAL_SCALE - 1.f)*m_rectangle.getLocalBounds().width)/2.f;

    m_rectangle.setScale(BOX_HORIZONTAL_SCALE, BOX_VERTICAL_SCALE);
    m_textsf.setPosition(WINDOW_SIZE + m_position.first + positionalShift,
                         MENUBAR_HEIGHT + m_position.second);
}

bool MoveBox::isHowered(coor2d& mousePos) {
    int x = mousePos.first - WINDOW_SIZE;
    int y = mousePos.second - MENUBAR_HEIGHT;

    return ((x > getPosition().first && x <= getPosition().first +  getScaledWidth()) 
         && (y > getPosition().second && y <= getPosition().second +  getScaledHeight()));
}

void MoveBox::setDefault() {
    m_textsf.setFillColor(Color(240, 248, 255)); // aliceblue
    m_rectangle.setFillColor(Color(50,50,50)); // charcoal
}

void MoveBox::setIsSelected() {
    m_textsf.setFillColor(Color(50,50,50));  // charcoal
    m_rectangle.setFillColor(Color(240, 248, 255)); // aliceblue
}


