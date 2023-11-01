#pragma once 

#include "Component.hpp"

#include <SFML/Graphics.hpp>

#include <vector>
#include <string>
#include <memory>
#include <functional>


namespace ui
{
    class Button : public Component
    {
        public:
            typedef std::function<void()> Callback;

            enum Type
            {
                Normal,
                Selected,
                Pressed,
                ButtonCount
            };

            explicit Button(const Callback& callback_);
            ~Button() override;

            void setCallback(const Callback& callback_);
            void setText(const std::string& text_);

            virtual bool isMouseInBounds() const override;
            virtual void doMouseClick() const;

            virtual void draw(sf::RenderTarget& target_, sf::RenderStates states_) const;

        protected:
            sf::RectangleShape	    m_rectangle;
            sf::Sprite				m_sprite;
            sf::Text				m_text;

        private:
            void changeTexture(Type buttonType_);
            Callback				m_callback;
    };
}