#include "../../include/Ressources/AudioManager.hpp"
#include "../../include/Ressources/RessourceManager.hpp"

void AudioManager::loadSound(SoundEffect effect_, const std::string& filename_)
{
    sf::SoundBuffer buffer;
    if (buffer.loadFromFile(RessourceManager::getAudioPath(filename_)))
    {
        m_sounds[effect_] = sf::Sound(buffer);
    }
}

void AudioManager::playSound(SoundEffect effect_)
{
    auto iter = m_sounds.find(effect_);
    if (iter != m_sounds.end())
    {
        iter->second.play();
    }
}
