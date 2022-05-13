#pragma once
#include <SFML/Graphics.hpp>
#include <string>

using namespace sf;

class DrawableSf
{
    public:
    virtual void drawTextSf(Text&, const std::string&, const Font*&, const int&, 
                    const Text::Style&, const Color&) const = 0;
    virtual void drawRectangleSf(RectangleShape&, const float&, const float&, 
                         const Vector2f&, const Color&) const = 0;
    virtual void drawSpriteSf(Sprite&, const float&, const float&, const float&, const float&,
                      const float&, const float&) const = 0; // will improve after testing this works
};