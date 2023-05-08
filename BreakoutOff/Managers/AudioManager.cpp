#include "AudioManager.h"
#include <iostream>
void AudioManager::init() {
	//Check if we call init function more than once
	if (initialized) {
		std::cout << "AudioManager::init called too many times" << std::endl;
		exit(0);
	}

	sfxSamples[SFX_Collide] = "res/Audio/collide.wav";
	sfxSamples[SFX_Win] = "res/Audio/win.wav";
	sfxSamples[SFX_Lose] = "res/Audio/lose.wav";
	sfxSamples[SFX_ButtonClick] = "res/Audio/buttonClick.wav";

	musicSamples[1] = "res/Audio/Music/digital-love-127441.wav";
	playMusic(1);
	initialized = true;
}
void AudioManager::playSFX(SFX type) {
	std::string fileName = getFile(type);
	if (fileName == "NULL") {
		return;
	}
	currBuffer.loadFromFile(fileName);
	sfxSound.setBuffer(currBuffer);
	sfxSound.play();
}
void AudioManager::playMusic(unsigned short levelIndex) {
	std::string fileName = getFile(levelIndex);
	if (fileName == "NULL") {
		return;
	}
	backgMusic.openFromFile(fileName);
	backgMusic.play();
	backgMusic.setLoop(true);
}
std::string AudioManager::getFile(SFX type) {
	if (sfxSamples[type] == "") {
		std::cout << "Couldnt find name " << type << " in sfxSamples" << std::endl;
		return "NULL";
	}
	return sfxSamples[type];
}
std::string AudioManager::getFile(unsigned short levelIndex) {
	if (musicSamples[levelIndex] == "") {
		std::cout << "Couldnt find file with index " << levelIndex << " in musicSamples" << std::endl;
		return "NULL";
	}
	return musicSamples[levelIndex];
}
void AudioManager::setVolumeSFX(float volume) {
	//Make sure that volume is clamped between 0 and 100
	this->sfxSound.setVolume(std::max(0.f, std::min(volume, 100.f)));
}
void AudioManager::setVolumeMusic(float volume) {
	//Make sure that volume is clamped between 0 and 100
	this->backgMusic.setVolume(std::max(0.f, std::min(volume, 100.f)));
}

void AudioManager::pauseMusic() {
	this->backgMusic.pause();
}
void AudioManager::resumeMusic() {
	this->backgMusic.play();
}
void AudioManager::restartMusic() {
	this->backgMusic.stop();
	this->backgMusic.play();
}
