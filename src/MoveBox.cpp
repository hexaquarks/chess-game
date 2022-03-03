#include "../include/MoveBox.hpp"

MoveBox::MoveBox(coor2d position, string text): m_position(position), m_text(text){
    // Font font;
    // if (!font.loadFromFile("../assets/fonts/Arial.ttf")) return;

    // handleText(*font);
};

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


