#include "../../include/Components/MoveSelectionPanel.hpp"

void MoveSelectionPanel::handleTitleText() {
    shared_ptr<Font> f = RessourceManager::getFont("Arial.ttf");
    m_title.setFont(*f);
    m_title.setString("Select a variation");
    m_title.setCharacterSize(14);
    m_title.setFillColor(Color::Black);
    m_title.setPosition(m_panel.getPosition() + Vector2f(10.f,10.f));
}

void MoveSelectionPanel::handlePanelRectangle() {
    // draw all the panel
    m_panel.setSize(Vector2f(200,300));
    m_panel.setFillColor(Color(211,211,211)); 
    m_panel.setPosition(WINDOW_SIZE + PANEL_SIZE / 3, PANEL_SIZE / 3);
}

void MoveSelectionPanel::handleTopRectangle() {
    // draw top left rectangle and title text
    m_topRect.setSize(Vector2f(200,50));
    m_topRect.setFillColor(Color(240, 248, 255));
    m_topRect.setPosition(m_panel.getPosition());
}

void MoveSelectionPanel::drawMoveSelectionPanel() {
    handlePanelRectangle();
    handleTopRectangle();
    handleTitleText();

    m_window.draw(m_panel);
    m_window.draw(m_topRect);
    m_window.draw(m_title);
}