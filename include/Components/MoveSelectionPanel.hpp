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
    MoveSelectionPanel(RenderWindow&);

    void handleTitleText();
    void handlePanelRectangle();
    void handleTopRectangle();
};