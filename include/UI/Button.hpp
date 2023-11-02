#pragma once 

#include "Component.hpp"

#include <SFML/Graphics.hpp>

#include <vector>
#include <string>
#include <memory>
#include <functional>

namespace ui
{
    typedef std::function<void()> Callback;

    class Button : public Component
    {
        public:

            enum Type
            {
                Normal,
                Selected,
                Pressed,
                ButtonCount
            };

            explicit Button(const std::string& name_, size_t index_, const Callback& callback_);
            explicit Button() = default;
            virtual ~Button() = default;

            void setCallback(const Callback& callback_);
            void setText(const std::string& text_);

            sf::RectangleShape getRectangle() const { return m_rectangle; }
            sf::Sprite getSprite() const { return m_sprite; }
            sf::Text getText() const { return m_text; }

            void setSpriteTexture(sf::Texture& texture_);

            virtual bool isMouseInBounds(coor2d& mousePos_) const override;
            virtual void doMouseClick() const override;

            virtual void draw(sf::RenderTarget& target_, sf::RenderStates states_) const;

        protected:
            sf::RectangleShape	    m_rectangle;
            sf::Sprite				m_sprite;
            sf::Text				m_text;

        private:
            void changeTexture(Type buttonType_);
            void rotateIcon();

            Callback				m_callback;
    };
}