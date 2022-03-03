#include "../include/RessourceManager.hpp"
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

void RessourceManager::addTexture(const string& name) { 
    shared_ptr<sf::Texture> texture = make_shared<sf::Texture>();
    texture->loadFromFile(getIconPath(name));
    m_textures.emplace(name, texture);
}

void RessourceManager::addFont(const string& name) { 
    shared_ptr<sf::Font> font = make_shared<sf::Font>();
    font->loadFromFile(getFontPath(name));
    m_fonts.emplace(name, font);
}

shared_ptr<sf::Texture> RessourceManager::getTexture(const string &name) {
    for (auto it = m_textures.begin(); it != m_textures.end(); ++it) {
        if (it->first == name) return it->second;
    }

    return nullptr;
};

shared_ptr<sf::Font> RessourceManager::getFont(const string &name) {
    for (auto it = m_fonts.begin(); it != m_fonts.end(); ++it) {
        if (it->first == name) return it->second;
    }

    return nullptr;
};

void RessourceManager::loadRessources() {
    // create the textures
    for (size_t i = 0; i < m_numberOfTextures; ++i) addTexture(m_textureNames[i]);
    // create the sounds 
    // create the fonts
    addFont(m_fontNames[0]);
}
