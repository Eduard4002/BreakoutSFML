#pragma once
#include <SFML/Graphics.hpp>
class Player
{

private:
	sf::RectangleShape shape;
	float velocity = 0.f;
	float speed = 400;
	sf::Vector2u windowSize;
	sf::Vector2f startPos;
public:
	Player(sf::RectangleShape &shape);
	Player(sf::Vector2f position, sf::Vector2f size, sf::Color color, sf::Vector2u windowSize);
	void Update(float deltaTime);
	void Render(sf::RenderTarget& window);

	sf::FloatRect getGlobalBounds();


	void Reset();
	sf::Vector2f getCurrPos();
};

