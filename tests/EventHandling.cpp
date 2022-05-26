#include <gtest/gtest.h>
#include "../include/GameThread.hpp"
#include "../include/Components/Board.hpp"
#include "../include/Pieces/Piece.hpp"

using namespace sf;

static RenderWindow window = {
        VideoMode(g_WINDOW_SIZE + g_PANEL_SIZE, g_WINDOW_SIZE + g_MENUBAR_HEIGHT),
        "Chess Game", Style::Titlebar | Style::Close
};

const Event simulateButtonPress(bool left, coor2d& mousePos)
{
    Event event;
    event.type = Event::MouseButtonPressed;
    event.mouseButton.button == left? Mouse::Left : Mouse::Right;
    Mouse::getPosition(window);
    return event;
}

TEST(BoardTests, EventHandlingTest)
{
    Board board;
    // Event handlers
    // simulateButtonPress();

}
