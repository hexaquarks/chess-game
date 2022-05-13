#pragma once
#include <SFML/Graphics.hpp>
#include <string>

using namespace sf;

class DrawableSf
{
    public:
    static void drawTextSf(Text&, const std::string&, Font&, const int&, 
                    const Text::Style&, const Color&);
    void drawRectangleSf(RectangleShape&, const float&, const float&, 
                         const Vector2f&, const Color&) const;
    void drawSpriteSf(Sprite&, const float&, const float&, const float&, const float&,
                      const float&, const float&) const; // will improve after testing this works
};