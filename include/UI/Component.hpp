#pragma once

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <memory>

typedef std::pair<int, int> coor2d;

namespace sf { class Event; }

namespace ui
{
    class Component : public sf::Drawable, 
                      public sf::Transformable
    {
        public:
            Component();
            virtual ~Component() = default;

            virtual bool isMouseInBounds(coor2d& mousePos_) const = 0; 
            virtual void doMouseClick() const;
    };
}