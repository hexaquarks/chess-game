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
    if (m_sounds.count(effect_) > 0)
    {
        m_sounds[effect_].play();
    }
}
