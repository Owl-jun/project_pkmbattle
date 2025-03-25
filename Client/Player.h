#pragma once

class Player {
private:
	sf::CircleShape shape;
public:
	Player();

	void move(const sf::Vector2f& offset);
	void draw(sf::RenderWindow& window);
};

