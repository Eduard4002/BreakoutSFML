#include "Player.h"
#include <iostream>
Player::Player(sf::RectangleShape &shape) {
	this->shape = shape;
}
Player::Player(sf::Vector2f position, sf::Vector2f size, sf::Color color, sf::Vector2u windowSize) {
	this->shape.setPosition(position);
	this->shape.setSize(size);
	this->shape.setFillColor(color);
	this->windowSize = windowSize;
	this->startPos = position;
}
sf::FloatRect Player::getGlobalBounds() {
	return this->shape.getGlobalBounds();
}
void Player::Update(float deltaTime) {
	velocity = 0.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		velocity = -speed * deltaTime;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		velocity = speed * deltaTime;
	}

	shape.move(velocity, 0.f);

	if (shape.getPosition().x < 0) {
		shape.setPosition(0, shape.getPosition().y);
	}
	else if (shape.getPosition().x + shape.getGlobalBounds().width > this->windowSize.x) {
		shape.setPosition(this->windowSize.x - shape.getGlobalBounds().width, shape.getPosition().y);
	}
}
void Player::Render(sf::RenderTarget& _window) {
	_window.draw(shape);
}

void Player::Reset() {
	this->shape.setPosition(startPos);
}
sf::Vector2f Player::getCurrPos() {
	return this->shape.getPosition();
}
