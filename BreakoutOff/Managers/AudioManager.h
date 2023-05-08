#include "SFML/Audio.hpp"
#include <map>
#pragma once
enum SFX {
	SFX_Collide,
	SFX_Win,
	SFX_Lose,
	SFX_ButtonClick
};
class AudioManager
{
private:
		   //level index	 file path
	std::map<unsigned short, std::string> musicSamples;
		   //SFX name file path
	std::map<SFX,     std::string> sfxSamples;

	sf::SoundBuffer currBuffer;
	sf::Sound sfxSound;
	
	sf::Music backgMusic;

	bool initialized = false;

private:
	std::string getFile(SFX type);
	std::string getFile(unsigned short levelIndex);

public:
	static AudioManager& getInstance() {
		static AudioManager instance;
		return instance;
	}
	void init();


	void setVolumeSFX(float volume);
	void setVolumeMusic(float volume);

	//More background music stuff
	void pauseMusic();
	void resumeMusic();
	void restartMusic();
	void playMusic(unsigned short levelIndex);
	void playSFX(SFX type);

private:
	//For Singleton pattern
	AudioManager() {}
	AudioManager(const AudioManager&) = delete;
	AudioManager& operator=(const AudioManager&) = delete;
};

