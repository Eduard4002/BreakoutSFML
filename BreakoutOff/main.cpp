#include <SFML\Graphics.hpp>

#include "Managers/GameManager.h"
int main() {
	sf::Clock clock;
	sf::RenderWindow window(sf::VideoMode(912, 512), "BREAKOUT", sf::Style::Close);
	window.setFramerateLimit(144);
	GameManager& gameManager = GameManager::getInstance();
	gameManager.init(window);

	while (window.isOpen()) {
		sf::Event evnt;
		while (window.pollEvent(evnt)) {
			gameManager.ProcessEvent(evnt);
			if (evnt.type == sf::Event::Closed) {
				window.close();
			}
		}
		gameManager.Update(clock.restart());

	}
	gameManager.ShutDown();
}