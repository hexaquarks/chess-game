#include "../../include/UI/Component.hpp"

namespace UI
{
    Component::Component() : m_isSelected(false) , m_isActive(false)
    {
    }

    Component::~Component()
    {
    }

    bool Component::isSelected() const
    {
        return m_isSelected;
    }

    void Component::select()
    {
        m_isSelected = true;
    }

    void Component::deselect()
    {
        m_isSelected = false;
    }

    bool Component::isActive() const
    {
        return m_isActive;
    }

    void Component::activate()
    {
        m_isActive = true;
    }

    void Component::deactivate()
    {
        m_isActive = false;
    }

}