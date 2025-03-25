#include "pch.h"
#include "Player.h"

Player::Player()
{
	shape.setRadius(50);
	shape.setFillColor(sf::Color::Green);
}

void Player::move(const sf::Vector2f& offset)
{
	shape.move(offset);
}

void Player::draw(sf::RenderWindow& window)
{
	window.draw(shape);
}
