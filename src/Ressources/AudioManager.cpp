#include "../../include/Ressources/AudioManager.hpp"
#include "../../include/Ressources/RessourceManager.hpp"

void AudioManager::loadAllSounds()
{
    loadSound(SoundEffect::MOVE, "move.wav");
    loadSound(SoundEffect::CAPTURE, "captures.wav");
}

void AudioManager::loadSound(SoundEffect effect_, const std::string& filename_)
{
    sf::SoundBuffer buffer;
    if (buffer.loadFromFile(RessourceManager::getAudioPath(filename_)))
    {
        m_sounds[effect_] = sf::Sound(buffer);
    }
}

bool AudioManager::canPlaySound(SoundEffect effect_) const
{
    auto iter = m_sounds.find(effect_);
    return (iter != m_sounds.end());
}

size_t AudioManager::getNumberOfLoadedSounds() const noexcept {
    return m_sounds.size();
}

void AudioManager::playSound(SoundEffect effect_)
{
    auto iter = m_sounds.find(effect_);
    if (iter != m_sounds.end())
    {
        iter->second.play();
    }
}
