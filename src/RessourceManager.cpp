#include "../include/RessourceManager.hpp"
#include <vector>
#include <algorithm>

using namespace std;

RessourceManager::RessourceManager() {
    // create the textures
    for(size_t i = 0; i < 2; ++i) {
        addTexture(m_textureNames[i]);
    }
    // create the sounds 
};

void RessourceManager::addTexture(const string& name) {
    // Check if texture already exists in the vector 
    if (find(m_textures.begin(), m_textures.end(), name) == m_textures.end())
        return;

    unique_ptr<sf::Texture> texture = make_unique<sf::Texture>();
    texture->loadFromFile(getIconPath(name));
    m_textures.emplace_back(name, move(texture)); // not sure ? 

};

unique_ptr<sf::Texture> RessourceManager::getTexture(const string &name) {
    textureVector::iterator it;
    it = find(m_textures.begin(), m_textures.end(), name);
 
    if (it != m_textures.end()) return nullptr;

    return move(it->second);
};