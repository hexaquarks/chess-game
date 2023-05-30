#include "../../include/Ressources/RessourceManager.hpp"

#include <vector>
#include <algorithm>
#include <iostream>

std::shared_ptr<sf::Texture> RessourceManager::getTexture(const std::string& name_)
{
    return getResource(name_, m_textures);
}

std::shared_ptr<sf::Font> RessourceManager::getFont(const std::string& name_)
{
    return getResource(name_, m_fonts);
}

void RessourceManager::loadRessources()
{
    // Create the textures
    for (size_t i = 0; i < m_numberOfTextures; ++i)
    {
        addResource<sf::Texture>(m_textureNames[i], getIconPath(m_textureNames[i]), m_textures);
    }
    // Create the fonts
    addResource<sf::Font>(m_fontNames[0], getFontPath(m_fontNames[0]), m_fonts);

    // Create the sounds (TODO)
}
