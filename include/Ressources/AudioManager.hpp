#include <SFML/Audio.hpp>
#include <string>
#include <unordered_map>

enum class SoundEffect 
{ 
    MOVE, 
    CAPTURE 
};

class AudioManager
{
public:
    // Singleton pattern
    static AudioManager& getInstance()
    {
        static AudioManager instance;
        return instance;
    }

    void loadSound(SoundEffect effect_, const std::string& filename_);
    void playSound(SoundEffect effect_);

private:
    std::unordered_map<SoundEffect, sf::Sound> m_sounds;

    // Rule of zero - sf::Sound and sf::SoundBuffer already manage
    // ressources correctly.
    AudioManager() = default;
    ~AudioManager() = default;

    // Rule of five applied to singleton pattern.
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;
};