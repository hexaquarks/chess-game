#pragma once

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <memory>

namespace sf { class Event; }

namespace ui
{
    class Component : public sf::Drawable, 
                      public sf::Transformable
    {
        public:
            Component();
            virtual ~Component();

            virtual bool isMouseInBounds() const; 
            virtual void doMouseClick() const;
    };
}