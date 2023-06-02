#include "../../include/Utilities/SFDrawUtil.hpp"

void SFDrawUtil::drawTextSf(
    Text& texture_, const std::string& text_, Font& font_, int size_,
    const Text::Style& style_, const Color& color_
)
{
    texture_.setString(text_);
    texture_.setFont(font_);
    texture_.setCharacterSize(size_);
    texture_.setStyle(style_);
    texture_.setFillColor(color_);
}

void SFDrawUtil::drawRectangleSf(
    RectangleShape& rect_, float posX_, float posY_,
    const Vector2f& size_, const Color& color_
)
{
    rect_.setPosition(posX_, posY_);
    rect_.setSize(size_);
    rect_.setFillColor(color_);
}
