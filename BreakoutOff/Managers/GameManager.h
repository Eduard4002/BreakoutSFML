#pragma once
#include <SFML\Graphics.hpp>
#include "UIManager.h"
#include "AudioManager.h"
#include "../Objects/Player.h"
#include "../Objects/Ball.h"
#include <vector>
class UIManager;
class Ball;
struct Level {
	short index;
	bool board[11][11];
};
class GameManager
{
private:
	std::vector<Level> allLevels;
	std::vector<sf::RectangleShape> boardObjects;
	UIManager* UI;
	AudioManager& audioManager = AudioManager::getInstance();
	sf::RenderWindow* mainWindow;
	Player* player;
	Ball* ball;

	Level currLevel;
	bool initialized = false;

	bool levelStarted = false;

	sf::Vector2f boardObjectsOffset = { 7.5,5 };
	sf::Vector2u boardObjectsPadding = { 10,10 };
	sf::Vector2u boardObjectsSize = { 75,20 };

	sf::Clock internalClock;
	//Shader
	sf::RectangleShape shaderShape;
	sf::Shader shader;

	bool shaderAvailable = false;
private:
	void LoadLevelInfo(Level level);
	bool getBoardValue(short x, short y, short levelIndex);
	GameManager() {};
	GameManager(const GameManager&) = delete;
	GameManager& operator=(const GameManager&) = delete;
public:
	static GameManager& getInstance() {
		static GameManager instance;
		return instance;
	}

	void init(sf::RenderWindow& window);
	void Update(sf::Time deltaTime);

	void ShutDown();
	void ProcessEvent(sf::Event);

	void NextLevel();
	void setLevel(short index);
	void RestartLevel();
	
	float getRandCol();
};

