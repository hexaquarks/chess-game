#include "../../include/Components/MoveSelectionPanel.hpp"

MoveSelectionPanel::MoveSelectionPanel(RenderWindow& window): m_window(window) {
    handlePanelRectangle();
    handleTopRectangle();
    handleTitleText();
}

void MoveSelectionPanel::handleTitleText() {
    shared_ptr<Font> f = RessourceManager::getFont("Arial.ttf");
    Text title;
    title.setFont(*f);
    title.setString("Select a variation");
    title.setCharacterSize(14);
    title.setFillColor(Color::Black);
    title.setPosition(m_panel.getPosition() + Vector2f(10.f,10.f));
}

void MoveSelectionPanel::handlePanelRectangle() {
    // draw all the panel
    RectangleShape m_panel(Vector2f(200,300));
    m_panel.setFillColor(Color(211,211,211)); 
    m_panel.setPosition(WINDOW_SIZE + PANEL_SIZE / 3, PANEL_SIZE / 3);
}

void MoveSelectionPanel::handleTopRectangle() {
    // draw top left rectangle and title text
    RectangleShape m_topRect(Vector2f(200,50));
    m_topRect.setFillColor(Color(240, 248, 255));
    m_topRect.setPosition(m_panel.getPosition());
}