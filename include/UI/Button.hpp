#pragma once 

#include "Component.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <vector>
#include <string>
#include <memory>
#include <functional>


namespace UI
{
    class Button : public Component
    {
        public:
            typedef std::function<void()>		Callback;

            enum Type
            {
                Normal,
                Selected,
                Pressed,
                ButtonCount
            };

            Button(Callback callBack_, sf::Sprite sprite_, sf::Text text_);

            void setCallback(Callback callback_);
            void setText(const std::string& text_);
            void setToggle(bool flag_);

            virtual bool isSelectable() const;
            virtual void select();
            virtual void deselect();

            virtual void activate();
            virtual void deactivate();

        private:
            virtual void draw(sf::RenderTarget& target_, sf::RenderStates states_) const;
            void changeTexture(Type buttonType_);

        private:
            Callback				m_callback;
            sf::Sprite				m_sprite;
            sf::Text				m_text;
            bool					m_isToggle;
    };
}