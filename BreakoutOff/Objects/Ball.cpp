#include "Ball.h"

Ball::Ball(int radius, float ballSpeed, sf::Color fillColor, sf::Vector2f position, sf::Vector2u windowSize)
{
	shape.setRadius(radius);
	shape.setPosition(position);
	shape.setFillColor(fillColor);
	//ballVelocity = sf::Vector2f(-0.08f, -0.1f);
	//ballVelocity = sf::Vector2f(position.x > windowSize.x ? 0.08f : -0.08f, position.y > windowSize.y ? -0.1f : 0.1f);
	this->ballSpeed = ballSpeed;
	this->windowSize = windowSize;
	this->startPos = position;
	this->startVelocity = ballVelocity;
}
Ball::Ball(sf::CircleShape& shape, sf::Vector2u windowSize)
{
	this->shape = shape;
	this->windowSize = windowSize;
	ballVelocity = sf::Vector2f(-0.08f, -0.08f);
	this->startPos = shape.getPosition();

}

void Ball::Update(float deltaTime, Player& player) {
	//left side
	if (shape.getPosition().x < 0) {
		ballVelocity.x *= -1;
	}
	//right side
	else if (shape.getPosition().x + shape.getRadius() * 2 > this->windowSize.x ) {
		ballVelocity.x *= -1;
	}
	//top side
	else if (shape.getPosition().y  < 0) {
		ballVelocity.y *= -1;
	}
	

	if (checkCollision(player.getGlobalBounds(), deltaTime)) {
		RotateToCenter();
	}
	shape.move(ballVelocity * ballSpeed * deltaTime);
	
}
void Ball::Render(sf::RenderTarget& window) {
	window.draw(this->shape);
}
bool Ball::checkCollision(sf::FloatRect obj1, float deltaTime) {
	nextPos = shape.getGlobalBounds();
	nextPos.top += ballVelocity.y * ballSpeed * deltaTime *2.5f;
	nextPos.left += ballVelocity.x * ballSpeed * deltaTime*2.5f;
	if (obj1.intersects(nextPos)) 
	{
		return true;
	}
	return false;
}


void Ball::RotateToCenter() {
	float dx = abs(nextPos.left - (this->windowSize.x  - rand() % 60));
	float dy = abs(nextPos.top - (this->windowSize.y - rand() % 60));
	
	float rotation = (atan2(dy, dx)) * (180 / PI);

	shape.rotate(rotation);
	ballVelocity.y *= -1;
}

void Ball::Reset() {
	this->shape.setPosition(startPos);
	this->ballVelocity = startVelocity;

}
void Ball::setPosition(sf::Vector2f pos) {
	this->shape.setPosition(pos);
	ballVelocity = sf::Vector2f(pos.x > windowSize.x * 0.5f? -0.08f : 0.08f, -0.1f);
}
bool Ball::outsideOfWindow() {
	return shape.getPosition().y + shape.getRadius() * 2 > this->windowSize.y;
}
void Ball::setColor(sf::Color color) {
	this->shape.setFillColor(color);
}