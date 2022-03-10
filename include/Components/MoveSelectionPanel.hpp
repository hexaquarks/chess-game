#pragma once
#include <SFML/Graphics.hpp>
#include "../GameThread.hpp"

using namespace sf;

class MoveSelectionPanel {
    RenderWindow& m_window;
    Text m_title;
    RectangleShape m_panel;
    RectangleShape m_topRect;

    public:
    MoveSelectionPanel(RenderWindow& window): m_window(window) {};

    void handleTitleText();
    void handlePanelRectangle();
    void handleTopRectangle();
    void drawMoveSelectionPanel();
};