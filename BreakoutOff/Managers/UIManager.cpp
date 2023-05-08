#include "UIManager.h"
#include "SFML\Graphics.hpp"
#include <iostream>

UIManager::UIManager(sf::RenderWindow& _window) : gameManager(GameManager::getInstance()) {
	this->window = &_window;
	defaultWindowFlags = ImGuiWindowFlags_NoTitleBar |
		                 ImGuiWindowFlags_NoMove | 
		                 ImGuiWindowFlags_NoResize;
	ImGui::SFML::Init(_window, false);

	ImGui::GetIO().Fonts->AddFontFromFileTTF("res/Font/ChakraPetch-Medium.ttf", 48);
		
	ImGui::SFML::UpdateFontTexture();

	//Construct different panels
	constructPanels(PanelType_MainPanel,     this->window->getSize(), sf::Vector2f(0, 0), this->defaultWindowFlags|ImGuiWindowFlags_NoBackground);
	constructPanels(PanelType_GameOverPanel, this->window->getSize(), sf::Vector2f(0, 0), this->defaultWindowFlags);
	constructPanels(PanelType_WinPanel,      this->window->getSize(), sf::Vector2f(0, 0), this->defaultWindowFlags);
	constructPanels(PanelType_PausePanel,    this->window->getSize(), sf::Vector2f(0, 0), this->defaultWindowFlags);
	constructPanels(PanelType_SettingsPanel, this->window->getSize(), sf::Vector2f(0, 0), this->defaultWindowFlags);
	constructPanels(PanelType_LastPanel,     this->window->getSize(), sf::Vector2f(0, 0), this->defaultWindowFlags);
	constructPanels(PanelType_MainMenu,      this->window->getSize(), sf::Vector2f(0, 0), this->defaultWindowFlags);
	constructPanels(PanelType_InfoPanel,     this->window->getSize(), sf::Vector2f(0, 0), this->defaultWindowFlags);


	//Set default colors
	this->fillColor =         ImVec4((float)135 / 255, (float)135 / 255, (float)135 / 255, 1);
	this->hoverColor =        ImVec4((float)64 / 255, (float)65 / 255, (float)66 / 255, 1);
	this->activeColor =       ImVec4((float)39 / 255, (float)39 / 255, (float)41 / 255, 1);
	this->buttonTextColor =   ImVec4((float)26 / 255, (float)26 / 255, (float)26 / 255, 1);
	this->textColor =         ImVec4(1, 1, 1, 1);
	this->timer.getElapsedTime();

	this->audioManager.setVolumeMusic(musicVolume);
	this->audioManager.setVolumeSFX(sfxVolume);

}

void UIManager::constructPanels(PanelType type, sf::Vector2u size, sf::Vector2f pos, ImGuiWindowFlags windowFlags) {
	Panel temp;
	temp.type = type;
	temp.size = size;
	temp.position = pos;
	temp.windowFlags = windowFlags;
	allPanels.push_back(temp);
}
Panel& UIManager::getPanel(PanelType type) {
	for (int i = 0; i < allPanels.size(); i++) {
		if (allPanels[i].type == type) {
			return allPanels[i];
		}
	}
	//if none found
	Panel temp;
	return temp; 
}

void UIManager::createText(const char* message, sf::Vector2f position, sf::Color fillColor = sf::Color::Black) {
	//Calculate the size of the text
	ImVec2 size = ImGui::CalcTextSize(message);
	//set cursor positon, this is where we will start drawing text [subtract half width to center the text]
	ImGui::SetCursorPos(ImVec2(position.x - size.x * 0.5, position.y));
	//create a normal text with default color
	if (fillColor == sf::Color::Black) {
		ImGui::TextColored(this->textColor, message);
	}
	else {
		ImGui::TextColored((ImVec4)fillColor, message);
	}

}
bool UIManager::createButton(const char* message, sf::Vector2f size, sf::Vector2f position, ImVec4 _fillColor = sf::Color::Black, ImVec4 _hoverColor = sf::Color::Black, ImVec4 _pressedColor = sf::Color::Black) {
	//set cursor position, this is where we will start drawing text [subtract half width to center the button]
	ImGui::SetCursorPos(ImVec2(position - sf::Vector2f(size.x / 2, 0)));
	//BUTTONS
	if (_fillColor == sf::Color::Black) {
		//Set default color
		ImGui::PushStyleColor(ImGuiCol_Button, this->fillColor);
	}else {
		ImGui::PushStyleColor(ImGuiCol_Button, _fillColor);
	}

	if (_hoverColor == sf::Color::Black) {
		//Set default color

		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, this->hoverColor);
	}else {
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, _hoverColor);
	}

	if (_pressedColor == sf::Color::Black) {
		//Set default color

		ImGui::PushStyleColor(ImGuiCol_ButtonActive, this->activeColor);
	}else {
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, _pressedColor);
	}
	//For rounding edges
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);
	ImGui::PushStyleColor(ImGuiCol_Text, buttonTextColor);

	if (ImGui::Button(message, size)) {
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();

		audioManager.playSFX(SFX_ButtonClick);
		return true;
	}
	else {
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();

		return false;
	}
	
}
void UIManager::createSliderInt(const char* label, sf::Vector2f position, int* val, int min, int max, ImVec4 _fillColor = sf::Color::Black, ImVec4 _hoverColor = sf::Color::Black, ImVec4 _pressedColor = sf::Color::Black) {
	ImGui::PushStyleColor(ImGuiCol_SliderGrab, textColor);
	ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, textColor);

	if (_fillColor == sf::Color::Black) {
		ImGui::PushStyleColor(ImGuiCol_FrameBg, this->fillColor);
	}
	else {
		ImGui::PushStyleColor(ImGuiCol_FrameBg, _fillColor);
	}
	if (_hoverColor == sf::Color::Black) {
		ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, this->hoverColor);
	}
	else {
		ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, _hoverColor);
	}
	if (_pressedColor == sf::Color::Black) {
		ImGui::PushStyleColor(ImGuiCol_FrameBgActive, this->hoverColor);
	}
	else {
		ImGui::PushStyleColor(ImGuiCol_FrameBgActive, _pressedColor);
	}
	float size = ImGui::CalcItemWidth();

	ImGui::SetCursorPos(ImVec2(position - sf::Vector2f(size * 0.5f, 0)));
	ImGui::SliderInt(label, val, min, max, "");

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}
void UIManager::restartScene() {
	gameManager.RestartLevel();
	setPanel(PanelType_MainPanel);
}
void UIManager::ResetClock() {
	timer.restart();
}

void UIManager::setMainPanel() {
	Panel& panelValues = getPanel(PanelType_MainPanel);
	ImGui::Begin("Temp", NULL, panelValues.windowFlags);
	ImGui::SetWindowSize("Temp", panelValues.size);
	ImGui::SetWindowPos("Temp", panelValues.position);

	std::string string = "TIME: " + std::to_string((int)time);
	createText(string.c_str(), sf::Vector2f(window->getSize().x * 0.5f, 30));
	ImGui::End();
	 
}
void UIManager::setGameOverPanel() {
	if (!gameOverPanelCalled) {
		audioManager.playSFX(SFX_Lose);
		gameOverPanelCalled = true;
	}
	Panel& panelValues = getPanel(PanelType_GameOverPanel);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 1.f));
	ImGui::Begin("Temp", NULL, panelValues.windowFlags);

	ImGui::SetWindowSize("Temp", ImVec2(panelValues.size));
	ImGui::SetWindowPos("Temp", ImVec2(panelValues.position));
	createText("YOU LOST :[", sf::Vector2f(window->getSize().x * 0.5f, 30));
	bool restartPressed = createButton("RESTART", sf::Vector2f(175, 50), sf::Vector2f(this->window->getSize().x * 0.5, 200));
	if (restartPressed) {
		restartScene();
	}
	bool quitpressed = createButton("MAIN MENU", sf::Vector2f(225, 50), sf::Vector2f(this->window->getSize().x * 0.5, 300));
	if (quitpressed) {
		setPanel(PanelType_MainMenu);
	}
	ImGui::PopStyleColor();
	ImGui::End();

}
void UIManager::setWinPanel() {

	if (!winPanelCalled) {
		audioManager.playSFX(SFX_Win);
		winPanelCalled = true;
	}
	Panel& panelValues = getPanel(PanelType_WinPanel);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 1.f));
	ImGui::Begin("Temp", NULL, panelValues.windowFlags);

	ImGui::SetWindowSize("Temp", ImVec2(panelValues.size));
	ImGui::SetWindowPos("Temp", ImVec2(panelValues.position));
	createText("YOU WIN :]", sf::Vector2f(this->window->getSize().x * 0.5f, 20));

	std::string timeMsg = "TIME: " + std::to_string(time) + " sec";
	createText(timeMsg.c_str(), sf::Vector2f(this->window->getSize().x * 0.5f, 100));
	bool nextPressed = createButton("CONTINUE", sf::Vector2f(225, 50), sf::Vector2f(this->window->getSize().x * 0.5f, 250));
	if (nextPressed) {
		gameManager.NextLevel();
		if (currPanel != PanelType_LastPanel) {
			setPanel(PanelType_MainPanel);
		}
		
	}
	bool restartPressed = createButton("RESTART", sf::Vector2f(225, 50), sf::Vector2f(this->window->getSize().x * 0.5f, 325));
	if (restartPressed) {
		restartScene();
	}
	bool quitpressed = createButton("MAIN MENU", sf::Vector2f(225, 50), sf::Vector2f(this->window->getSize().x * 0.5f, 400));
	if (quitpressed) {
		setPanel(PanelType_MainMenu);
	}

	ImGui::PopStyleColor();

	ImGui::End();
}
void UIManager::setPausePanel()
{

	Panel& panelValues = getPanel(PanelType_PausePanel);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0.5f));

	ImGui::Begin("Temp", NULL, panelValues.windowFlags);
	ImGui::SetWindowSize("Temp", ImVec2(panelValues.size));
	ImGui::SetWindowPos("Temp", ImVec2(panelValues.position));
	createText("PAUSE MENU", sf::Vector2f(this->window->getSize().x * 0.5, 50));
	bool resumePressed = createButton("RESUME", sf::Vector2f(225, 50), sf::Vector2f(this->window->getSize().x * 0.5, 150));
	if (resumePressed) {
		setPanel(PanelType_MainPanel);
	}
	bool restartPressed = createButton("RESTART", sf::Vector2f(225, 50), sf::Vector2f(this->window->getSize().x * 0.5, 225));
	if (restartPressed) {
		restartScene();
	}
	bool settingsPressed = createButton("SETTINGS", sf::Vector2f(225, 50), sf::Vector2f(this->window->getSize().x * 0.5, 300));
	if (settingsPressed) {
		setPanel(PanelType_SettingsPanel);
	}
	bool quitpressed = createButton("MAIN MENU", sf::Vector2f(225, 50), sf::Vector2f(this->window->getSize().x * 0.5, 375));
	if (quitpressed) {
		setPanel(PanelType_MainMenu);
	}
	ImGui::PopStyleColor();
	ImGui::End();

}
void UIManager::setSettingsPanel() {
	Panel& panelValues = getPanel(PanelType_WinPanel);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 1.f));
	ImGui::Begin("Temp", NULL, panelValues.windowFlags);

	ImGui::SetWindowSize("Temp", ImVec2(panelValues.size));
	ImGui::SetWindowPos("Temp", ImVec2(panelValues.position));

	createText("SETTINGS MENU", sf::Vector2f(this->window->getSize().x * 0.5, 50));
	createSliderInt("SFX", sf::Vector2f(400, 150),&sfxVolume,0,100);
	createSliderInt("MUSIC", sf::Vector2f(400, 250), &musicVolume, 0, 100);
	bool backPressed = createButton("BACK", sf::Vector2f(175, 50), sf::Vector2f(this->window->getSize().x * 0.5, 400));
	if (backPressed) {
		setPanel(PanelType_PausePanel);
	}
	//Only if volume is different from last frame we can update AudioManager
	if (lastSfxVolume != sfxVolume) {
		audioManager.setVolumeSFX(sfxVolume);
	}
	lastSfxVolume = sfxVolume;

	if (lastMusicVolume != musicVolume) {
		audioManager.setVolumeMusic(musicVolume);
	}
	lastMusicVolume = musicVolume;

	ImGui::PopStyleColor();
	ImGui::End();
}
void UIManager::setLastPanel() {
	Panel& panelValues = getPanel(PanelType_LastPanel);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 1.f));

	ImGui::Begin("Temp", NULL, panelValues.windowFlags);
	ImGui::SetWindowSize("Temp", panelValues.size);
	ImGui::SetWindowPos("Temp", panelValues.position);

	createText("GOOD JOB!!!", { this->window->getSize().x * 0.5f,50});
	createText("THANK YOU SO MUCH FOR FINISHING MY GAME", { this->window->getSize().x * 0.5f,150 });
	createText("THIS IS MY FIRST GAME CREATED WITH SFML ", { this->window->getSize().x * 0.5f,200 });
	createText("AND THAT IS WHY ITS A BIT BUGGY", { this->window->getSize().x * 0.5f,250 });

	bool back = createButton("MAIN MENU", sf::Vector2f(225, 50), sf::Vector2f(this->window->getSize().x * 0.5, 400));
	if (back) {
		setPanel(PanelType_MainMenu);
	}

	ImGui::PopStyleColor();
	ImGui::End();
}
void UIManager::setMainMenuPanel() {

	Panel& panelValues = getPanel(PanelType_LastPanel);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 1.f));

	ImGui::Begin("Temp", NULL, panelValues.windowFlags);
	ImGui::SetWindowSize("Temp", panelValues.size);
	ImGui::SetWindowPos("Temp", panelValues.position);
	createText("MAIN MENU", { this->window->getSize().x * 0.5f,50 });
	bool startPressed = createButton("START", sf::Vector2f(225, 50), sf::Vector2f(this->window->getSize().x * 0.5, 225));
	if (startPressed) {
 		gameManager.setLevel(1);
		if (currPanel != PanelType_LastPanel) {
			setPanel(PanelType_MainPanel);
		}
	}
	bool infoPressed = createButton("INFO", sf::Vector2f(225, 50), sf::Vector2f(this->window->getSize().x * 0.5, 300));
	if (infoPressed) {
		setPanel(PanelType_InfoPanel);
	}
	bool quitpressed = createButton("QUIT", sf::Vector2f(225, 50), sf::Vector2f(this->window->getSize().x * 0.5, 375));
	if (quitpressed) {
		window->close();
	}
	


	ImGui::PopStyleColor();
	ImGui::End();
}
void UIManager::setInfoPanel() {
	Panel& panelValues = getPanel(PanelType_LastPanel);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 1.f));

	ImGui::Begin("Temp", NULL, panelValues.windowFlags);
	ImGui::SetWindowSize("Temp", panelValues.size);
	ImGui::SetWindowPos("Temp", panelValues.position);
	createText("Backg music: 'Digital love' by Alexi Action", { 375,25 });
	createText("Everything else programmed by me :)", { 350,100 });

	createText("Note: I did not implement save features", { 350,300 });
	createText("if you exit the program, your progress wont save", { 425,375 });

	bool backPressed = createButton("MAIN MENU", sf::Vector2f(225, 50), sf::Vector2f(this->window->getSize().x * 0.5, 450));
	if (backPressed) {
		setPanel(PanelType_MainMenu);
	}
	ImGui::PopStyleColor();
	ImGui::End();
}
void UIManager::Update(sf::Time timeClock) {

	ImGui::SFML::Update(*this->window, timeClock);
	
	switch (currPanel) {
	case PanelType_PausePanel:
		setPausePanel();
		break;
	case PanelType_MainPanel:
		time = timer.getElapsedTime().asSeconds();
		setMainPanel();
		break;
	case PanelType_GameOverPanel:
		setGameOverPanel();
		break;
	case PanelType_WinPanel:
		setWinPanel();
		break;
	case PanelType_SettingsPanel:
		setSettingsPanel();
		break;
	case PanelType_LastPanel:
		setLastPanel();
		break;
	case PanelType_MainMenu:
		setMainMenuPanel();
		break;
	case PanelType_InfoPanel:
		setInfoPanel();
		break;
	}

	//If ESC is pressed
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && !isPaused) {
		setPanel(PanelType_PausePanel);
	}
	ImGui::SFML::Render(*this->window);
	
}


void UIManager::setPanel(PanelType newPanel) {
	currPanel = newPanel;
	switch (newPanel) {
	case PanelType_MainPanel:
		isPaused = false;
		break;
	case PanelType_GameOverPanel:
		isPaused = true;
		break;
	case PanelType_WinPanel:
		isPaused = true;
		break;
	case PanelType_PausePanel:
		isPaused = true;
		break;
	case PanelType_LastPanel:
		isPaused = true;
		break;
	case PanelType_MainMenu:
		isPaused = true;
		break;
	}
}
void UIManager::ShutDown() {
	ImGui::SFML::Shutdown();
}
void UIManager::ProcessEvent(sf::Event evnt) {
	ImGui::SFML::ProcessEvent(*this->window, evnt);
}

