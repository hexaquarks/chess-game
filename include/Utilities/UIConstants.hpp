#pragma once

namespace ui 
{
    inline constexpr int g_MENUBAR_HEIGHT = 30;
    inline constexpr int g_NUMBER_BUTTONS = 6;

    inline constexpr int g_WINDOW_SIZE = 640;
    inline constexpr float g_SPRITE_SCALE = 0.6;
    inline constexpr float g_SPECIAL_SCALE = 1.0;

    inline constexpr int g_CELL_SIZE = g_WINDOW_SIZE / 8;
    inline constexpr int g_BUTTON_POS = g_WINDOW_SIZE / g_NUMBER_BUTTONS;
    inline constexpr int g_PANEL_SIZE = 640;
    inline constexpr int g_BORDER_SIZE = 10;
    inline constexpr int g_SOUTH_PANEL_HEIGHT = g_MENUBAR_HEIGHT;
    inline constexpr int g_MAIN_PANEL_HEIGHT = g_PANEL_SIZE - g_SOUTH_PANEL_HEIGHT;
    inline constexpr float g_SPRITE_SIZE = 128;
    inline constexpr float g_BUTTON_SIZE = 40;
}