#pragma once
#include "SFML\Graphics.hpp"
#include "../Managers/UIManager.h"
#include <iostream>
#include "Player.h"
class UIManager;
class GameManager;
class Ball
{
private:
	const float PI = 3.14159265;
	
	sf::CircleShape shape;
	sf::RectangleShape testShape;

	sf::Vector2f ballVelocity;
	float ballSpeed;
	sf::FloatRect nextPos;
	sf::Vector2u windowSize;

	sf::Vector2f startPos;
	sf::Vector2f startVelocity;

	float randColG;
public:
	Ball(int radius,float ballSpeed,sf::Color fillColor, sf::Vector2f position, sf::Vector2u windowSize);
	Ball(sf::CircleShape& shape, sf::Vector2u windowSize);

	void Update(float deltaTime, Player& player);
	void Render(sf::RenderTarget& window);
	
	bool checkCollision(sf::FloatRect, float deltaTime);
	void RotateToCenter();

	void Reset();
	void setPosition(sf::Vector2f pos);
	bool outsideOfWindow();
	void setColor(sf::Color color);
 };

