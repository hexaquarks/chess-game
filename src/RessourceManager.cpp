#include "../include/RessourceManager.hpp"
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

RessourceManager::RessourceManager() {};

void RessourceManager::addTexture(const string& name) { 
    shared_ptr<sf::Texture> texture = make_shared<sf::Texture>();
    texture->loadFromFile(getIconPath(name));
    m_textures.emplace(name, texture);
};

shared_ptr<sf::Texture> RessourceManager::getTexture(const string &name) {
    for(auto it = m_textures.begin();it != m_textures.end(); ++it) {
        if(it->first == name) return it->second;
    }

    return nullptr;
};

void RessourceManager::loadRessources() {
    // create the textures
    for(size_t i = 0; i < 2; ++i) addTexture(m_textureNames[i]);
    // create the sounds 
}