#include "../include/MoveBox.hpp"

MoveBox::MoveBox(coor2d position, string text): m_position(position), m_text(text) {};

void MoveBox::handleText(Font& font) {

    m_textsf.setString(m_text);
    m_textsf.setFont(font);
    m_textsf.setCharacterSize(25);
    m_textsf.setStyle(Text::Bold);
    m_textsf.setFillColor(Color::Black);

    m_textBounds = m_textsf.getGlobalBounds();
}

void MoveBox::handleRectangle() {
    Vector2f recSize(m_textBounds.width, m_textsf.getCharacterSize());
    m_rectangle.setPosition(WINDOW_SIZE + m_position.first, MENUBAR_HEIGHT + m_position.second);
    m_rectangle.setSize(recSize);
    m_rectangle.setFillColor(Color::White);
    m_rectangle.setOutlineThickness(2.f);

    m_textsf.setPosition(WINDOW_SIZE + m_position.first, MENUBAR_HEIGHT + m_position.second);
}

bool MoveBox::isHowered(coor2d& mousePos) {
    int x = mousePos.first - WINDOW_SIZE;
    int y = mousePos.second - MENUBAR_HEIGHT;

    return ((x > getPosition().first && x <= getPosition().first +  getTextBounds().width) &&
        (y > getPosition().second && y <= getPosition().second +  getTextBounds().height));
}

void MoveBox::setDefault() {
    m_textsf.setFillColor(Color::Black);
    m_rectangle.setFillColor(Color::White);
}

void MoveBox::setIsSelected() {
    m_textsf.setFillColor(Color::White);
    m_rectangle.setFillColor(Color::Black);
}


