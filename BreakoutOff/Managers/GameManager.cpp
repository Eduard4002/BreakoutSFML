#include "GameManager.h"
#include <array>


void GameManager::init(sf::RenderWindow& window) {
	//Check if we call init function more than once
	if (initialized) {
		std::cout << "GameManager::init called too many times" << std::endl;
		exit(0);
	}
	srand(time(NULL));

	
	if (sf::Shader::isAvailable) {
		if (!shader.loadFromFile("res/Shader/Shader.txt", sf::Shader::Fragment)) {
			//error
			std::cout << "Couldnt open file" << std::endl;
		}
		else {
			shaderShape.setSize(static_cast<sf::Vector2f>(window.getSize()));
			shader.setUniform("u_resolution", sf::Glsl::Vec2(window.getSize()));
			shaderAvailable = true;
		}
	}

	//Create levels
	Level temp;
	for (int i = 1; i <= 6; i++) {
		temp.index = i;
		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 11; j++) {
				temp.board[i][j] = getBoardValue(i, j, temp.index);
			}
		}
		allLevels.push_back(temp);

	}

	
	
	this->mainWindow = &window;
	this->audioManager.init();

	this->UI = new UIManager(window);
	internalClock.restart();
	UI->setPanel(PanelType_MainMenu);

	this->player = new Player(sf::Vector2f(392.5, 477), sf::Vector2f(100, 15), sf::Color::Blue, window.getSize());
	this->ball = new Ball(10, 3000, sf::Color::Red, sf::Vector2f(400, 460), window.getSize());
	initialized = true;
}

void GameManager::LoadLevelInfo(Level info) {

	//Delete previous info
	boardObjects.clear();
	levelStarted = false;

	this->UI->gameOverPanelCalled = false;
	this->UI->winPanelCalled = false;

	//Load new boardobjects 
	sf::RectangleShape tempShape;
	for (int i = 0; i < 11; i++) {
		for (int j = 0; j < 11; j++) {
			if (info.board[i][j] == true) {
				
				tempShape.setSize(sf::Vector2f(boardObjectsSize.x, boardObjectsSize.y));
				tempShape.setPosition(sf::Vector2f(boardObjectsPadding.x + j * (boardObjectsSize.x + boardObjectsOffset.x), 
												   boardObjectsPadding.y + i * (boardObjectsSize.y + boardObjectsOffset.y)));
				tempShape.setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));

				boardObjects.push_back(tempShape);

			}
		}
	}

	UI->ResetClock();

	//Set default position
	player->Reset();
	ball->Reset();

	this->currLevel = info;

}

void GameManager::Update(sf::Time deltaTime) {
	if (!levelStarted && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		levelStarted = true;
	}
	if (!UI->isPaused) {

		//std::cout << deltaTime << std::endl;
		
		for (int i = 0; i < boardObjects.size(); i++) {
			if (ball->checkCollision(boardObjects[i].getGlobalBounds(), deltaTime.asSeconds())) {
				boardObjects.erase(boardObjects.begin() + i);
				ball->RotateToCenter();
				audioManager.playSFX(SFX_Collide);
				//Check if player removed all objects
				if (boardObjects.size() == 0) {
					UI->setPanel(PanelType_WinPanel);
				}
			}

		}

		player->Update(deltaTime.asSeconds());

		if (levelStarted) {
			ball->Update(deltaTime.asSeconds(), *player);
			
		}
		else {
			//if we havent clicked then ball can follow player position
			ball->setPosition(sf::Vector2f(player->getCurrPos().x + 45, player->getCurrPos().y - 20));
		}

		if (ball->outsideOfWindow()) {
			this->UI->setPanel(PanelType_GameOverPanel);
		}
	}
	
	this->mainWindow->clear();

	this->player->Render(*mainWindow);
	this->ball->Render(*mainWindow);
	for (int i = 0; i < boardObjects.size(); i++) {
		this->mainWindow->draw(boardObjects[i]);
	}
	if (shaderAvailable) {
		this->mainWindow->draw(shaderShape, &shader);
	}
	this->UI->Update(deltaTime);

	this->mainWindow->display();

}
void GameManager::ShutDown() {
	UI->ShutDown();
}
void GameManager::ProcessEvent(sf::Event evnt) {
	UI->ProcessEvent(evnt);
}

void GameManager::NextLevel() {
	//Check if next level is available
	if (currLevel.index + 1 > allLevels.size()) {
		UI->setPanel(PanelType_LastPanel);
	}
	else {
		LoadLevelInfo(allLevels[currLevel.index]);
	}

}
void GameManager::setLevel(short levelIndex) {
	for (int i = 1; i < allLevels.size(); i++) {
		if (allLevels[i].index = levelIndex) {
			LoadLevelInfo(allLevels[i-1]);
			return;
		}
	}
}
void GameManager::RestartLevel() {
	LoadLevelInfo(currLevel);
}
float GameManager::getRandCol() {
	return rand() % 255;
}
bool GameManager::getBoardValue(short x, short y, short levelIndex){
	if (levelIndex == 1) {
		
		bool level1[11][11] = { {false, false, false, false, false, false, false, false,false,true,true},
								{false, false, false, true, false, false, true, false,true,true,true},
								{false, false, true, true, false, true, true, false,true,true,false},
								{false, true, true, false, true, true, false, false,true,false,false},
								{true, true, false, false, true, false, false, false,false,true,true},
								{true, false, false, false, false, false, true, false,false,true,false},
								{false, false, false, false, false, false, false, false,true,false,false},
								{false, false, true, false, false, true, false, true,true,false,true},
								{false, true, true, false, false, true, false, true,false,true,true},
								{true, true, false, false, true, false, true, false,true,true,false},
								{true, false, false, false, true, false, false, false,true,false,false},
		};
		return level1[x][y];
	}
	else if (levelIndex == 2) {
		bool level2[11][11] = { 
								{false, false, false, false, false, false, false, false,false,false,false},
								{false, true, true, true, true, true, true,   true,true,true,false},
								{false, true, true, true, true, true, true,   true,true,true,false},
								{false, true, true, true, false, false, false,true,true,true,false},
								{false, true, true, true, false, true, false, true,true,true,false},
								{false, true, true, true, true, true,  true,   true,true,true,false},
								{false, true, true, true, false, true, false, true,true,true,false},
								{false, true, true, true, false, false, false,true,true,true,false},
								{false, true, true, true, true, true, true,   true,true,true,false},
								{false, true, true, true, true, true, true,   true,true,true,false},
								{false, false, false, false, false, false, false, false,false,false,false},
								


		};
		return level2[x][y]; 
	}
	else if (levelIndex == 3) {
		bool level3[11][11] = { {false, false, false, false, false, false, false, false,false,false,false},
								{true, true, true, true, true, true, true,   true,true,true,true},
								{true, true, true, true, true, true, true,   true,true,true,true},
								{true, true, true, true, true, true, true,   true,true,true,true},
								{false, false, false, false, false, false, false, false,false,false,false},
								{true, true, true, true, true, true, true,   true,true,true,true},
								{true, true, true, true, true, true, true,   true,true,true,true},
								{true, true, true, true, true, true, true,   true,true,true,true},
								{false, false, false, false, false, false, false, false,false,false,false},
								{false, true, true, true, true, true, true,   true,true,true,false},
								{false, true, true, true, true, true, true,   true,true,true,false},
		};
		return level3[x][y];
	}
	else if (levelIndex == 4) {
		bool level4[11][11] = { {false, false, false, false, false, true, false, false,false,false,false},
								{false, false, false, false, true, true, true, false,false,false,false},
								{false, false, false, true, true, true, true, true,false,false,false},
								{false, false, true, true, true, false, true, true,true,false,false},
								{false, true, true, true, false, false, false, true,true,true,false},
								{true, true, true, false, false, false, false, false,true,true,true},
								{false, true, true, true, false, false, false, true,true,true,false},
								{false, false, true, true, true, false, true, true,true,false,false},
								{false, false, false, true, true, true, true, true,false,false,false},
								{false, false, false, false, true, true, true, false,false,false,false},
								{false, false, false, false, false, true, false, false,false,false,false},
		};
		return level4[x][y];
	}
	else if (levelIndex == 5) {
		bool level5[11][11] = { {true, true, true, true, true, true, true, true,true,true,true},
								{true, true, true, false, false, false, false, false,true,true,true},
								{true, true, true, true, false, false, false, true,true,true,true},
								{true, false, true, true, true, false, true, true,true,false,true},
								{true, true, false, true, true, true, true, true,false,true,true},
								{true, false, true, false, true, true, true, false,true,false,true},
								{false, false, false, true, false, false, false, true,false,false,false},
								{false, false, false, false, false, false, false, false,false,false,false},
								{false, false, false, false, false, false, false, false,false,false,false},
								{false, false, false, false, false, false, false, false,false,false,false},
								{false, false, false, false, false, false, false, false,false,false,false},

		};
		return level5[x][y];
	}
	else if (levelIndex == 6) {
		bool level6[11][11] = { 
								{false, false, false, false, false, false, false, false,false,false,false},
								{false, true, true, true, true, true, true, true,true,true,false},
								{false, false, true, true, true, true, true, true,true,false,false},
								{false, false, false, true, true, true, true, true,false,false,false},
								{false, false, false, false, true, true, true, false,false,false,false},
								{false, false, false, true, true, true, true, true,false,false,false},
								{false, false, true, true, true, true, true, true,true,false,false},
								{false, true, true, true, true, true, true, true,true,true,false},
								{false, false, false, false, false, false, false, false,false,false,false},
								{false, false, false, false, false, false, false, false,false,false,false},
								{false, false, false, false, false, false, false, false,false,false,false},
		};
		return level6[x][y];
	}
	else {
		std::cout << "Level Index not found" << std::endl;
	}
	
	
}