#include "../../include/Components/MoveBox.hpp"

MoveBox::MoveBox(coor2d position_, string text_): m_position(position_), m_text(text_) {};

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
    m_rectangle.setFillColor({ 50, 50, 50 });

    float positionalShift = ((BOX_HORIZONTAL_SCALE - 1.f)*m_rectangle.getLocalBounds().width)/2.f;

    m_rectangle.setScale(BOX_HORIZONTAL_SCALE, BOX_VERTICAL_SCALE);
    m_textsf.setPosition(WINDOW_SIZE + m_position.first + positionalShift,
                         MENUBAR_HEIGHT + m_position.second);
}

bool MoveBox::isHowered(coor2d& mousePos_) const {
    int x = mousePos_.first - WINDOW_SIZE;
    int y = mousePos_.second - MENUBAR_HEIGHT;

    return ((x > getPosition().first && x <= getPosition().first +  getScaledWidth()) 
         && (y > getPosition().second && y <= getPosition().second +  getScaledHeight()));
}

void MoveBox::setDefault() {
    m_textsf.setFillColor({ 240, 248, 255 }); // aliceblue
    m_rectangle.setFillColor({ 50, 50, 50 }); // charcoal
}

void MoveBox::setIsSelected() {
    m_textsf.setFillColor({ 240, 248, 255 });  // charcoal
    m_rectangle.setFillColor({ 139, 148, 158 }); // aliceblue
}

void MoveBox::setIsCurrentMove() {
    m_textsf.setFillColor({ 50, 50, 50 });  // charcoal
    m_rectangle.setFillColor({ 240, 248, 255 }); // aliceblue
}

