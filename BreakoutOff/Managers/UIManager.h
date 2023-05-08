#pragma once
#include <SFML\Graphics.hpp>
#include "GameManager.h"
#include "AudioManager.h"
#include <vector>
#include "IMGUI_SFML\imgui.h"

#include "IMGUI_SFML\imgui-SFML.h"
class GameManager;

enum PanelType {
	PanelType_MainPanel,
	PanelType_GameOverPanel,
	PanelType_WinPanel,
	PanelType_PausePanel,
	PanelType_SettingsPanel,
	PanelType_LastPanel,
	PanelType_MainMenu,
	PanelType_InfoPanel
};
struct Panel {
public:
	PanelType type;
	sf::Vector2u size;
	sf::Vector2f position;
	ImGuiWindowFlags windowFlags;

};
class UIManager
{
private:
	sf::RenderWindow* window;
	GameManager& gameManager;
	AudioManager& audioManager = AudioManager::getInstance();

	std::vector<Panel> allPanels;
	PanelType currPanel = PanelType_MainPanel;
	ImGuiWindowFlags defaultWindowFlags;

	//Stopwatch for mainPanel
	sf::Clock timer;
	int time;

	//Default colors
	ImVec4 fillColor;
	ImVec4 hoverColor;
	ImVec4 activeColor;
	ImVec4 textColor;

	ImVec4 buttonTextColor;

	//Settings panel stuff
	int sfxVolume = 100;
	int lastSfxVolume = 100;

	int musicVolume = 15;
	int lastMusicVolume = 100;

	
public:
	bool isPaused = false;

	bool gameOverPanelCalled = false;
	bool winPanelCalled = false;
public:
	UIManager(sf::RenderWindow& window);
	void Update(sf::Time timeClock);
	void setPanel(PanelType newPanel);
	void ShutDown();
	void ProcessEvent(sf::Event evnt);
	void ResetClock();
private:
	void createText  (const char* message, sf::Vector2f position, sf::Color fillColor);
	bool createButton(const char* message, sf::Vector2f size, sf::Vector2f position, ImVec4 fillColor, ImVec4 hoverColor,ImVec4 pressedColor);
	void createSliderInt(const char* label, sf::Vector2f position,int* val, int min, int max, ImVec4 fillColor, ImVec4 hoverColor, ImVec4 pressedColor);
	
	void setMainPanel();
	void setGameOverPanel();
	void setWinPanel();
	void setPausePanel();
	void setSettingsPanel();
	void setLastPanel();
	void setMainMenuPanel();
	void setInfoPanel();

	void restartScene();

	void constructPanels(PanelType type, sf::Vector2u size, sf::Vector2f pos, ImGuiWindowFlags windowFlags);
	Panel& getPanel(PanelType type);

};

 


