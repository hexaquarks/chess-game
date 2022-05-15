#include "../../include/Utilities/DrawableSf.hpp"

void DrawableSf::drawTextSf(Text& texture_, const std::string& text_, Font& font_, const int& size_, 
                    const Text::Style& style_, const Color& color_)
{
    texture_.setString(text_);
    texture_.setFont(font_);
    texture_.setCharacterSize(28);
    texture_.setStyle(Text::Bold);
    texture_.setFillColor({ 240, 248, 255 });
}

void DrawableSf::drawRectangleSf(RectangleShape& rect_, const float& posX_, const float& posY_, 
                         const Vector2f& size_, const Color& color_)
{
    rect_.setPosition(posX_, posY_);
    rect_.setSize(size_);
    rect_.setFillColor(color_);
}                         