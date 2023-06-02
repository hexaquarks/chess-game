#pragma once
#include <SFML/Graphics.hpp>
#include <string>

using namespace sf;

class SFDrawUtil
{
public:
    static void drawTextSf(
        Text&, const std::string&, Font&, int,
        const Text::Style&, const Color&
    );
    static void drawRectangleSf(
        RectangleShape&, float, float,
        const Vector2f&, const Color&
    );
    static void drawSpriteSf(
        Sprite&, float, float, float, float,
        float, float
    ); // Will improve after testing this works
};
