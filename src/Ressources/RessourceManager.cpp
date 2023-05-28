#include "../../include/Ressources/RessourceManager.hpp"

#include <vector>
#include <algorithm>
#include <iostream>

void RessourceManager::addTexture(const std::string& name_)
{
    std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();
    texture->loadFromFile(getIconPath(name_));
    m_textures.emplace(name_, texture);
}

void RessourceManager::addFont(const std::string& name_)
{
    std::shared_ptr<sf::Font> font = std::make_shared<sf::Font>();
    font->loadFromFile(getFontPath(name_));
    m_fonts.emplace(name_, font);
}

template<typename T>
std::shared_ptr<T> getObject(const std::string& name_, const std::unordered_map<std::string, std::shared_ptr<T>>& map) {
    for (auto it = map.begin(); it != map.end(); ++it)
    {
        if (it->first == name_) return it->second;
    }
    return {};
}

std::shared_ptr<sf::Texture> RessourceManager::getTexture(const std::string& name_)
{
    return getObject(name_, m_textures);
}

std::shared_ptr<sf::Font> RessourceManager::getFont(const std::string& name_)
{
    return getObject(name_, m_fonts);
}

void RessourceManager::loadRessources()
{
    // Create the textures
    for (size_t i = 0; i < m_numberOfTextures; ++i)
        addTexture(m_textureNames[i]);

    // Create the sounds (not yet implemented)

    // Create the fonts
    addFont(m_fontNames[0]);
}
