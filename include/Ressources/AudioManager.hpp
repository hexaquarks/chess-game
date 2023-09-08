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

    void loadAllSounds();
    void playSound(SoundEffect effect_) const;
    bool canPlaySound(SoundEffect effect_) const;
    size_t getNumberOfLoadedSounds() const noexcept;

private:
    std::unordered_map<SoundEffect, sf::Sound> m_sounds;

    void loadSound(SoundEffect effect_, const std::string& filename_);

    // Rule of zero - sf::Sound and sf::SoundBuffer already manage
    // ressources correctly.
    AudioManager() = default;
    ~AudioManager() = default;

    // Rule of five applied to singleton pattern.
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;
};