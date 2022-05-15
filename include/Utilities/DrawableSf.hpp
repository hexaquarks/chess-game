#pragma once
#include <SFML/Graphics.hpp>
#include <string>

using namespace sf;

class DrawableSf
{
    public:
    static void drawTextSf(Text&, const std::string&, Font&, const int&, 
                    const Text::Style&, const Color&);
    static void drawRectangleSf(RectangleShape&, const float&, const float&, 
                         const Vector2f&, const Color&);
    static void drawSpriteSf(Sprite&, const float&, const float&, const float&, const float&,
                      const float&, const float&); // will improve after testing this works
};