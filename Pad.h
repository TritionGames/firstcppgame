#pragma once

#include "SFML/Graphics.hpp"

class Pad {
private:
	sf::RectangleShape rect;

	sf::Vector2f pos = sf::Vector2f(50.f, 200.f);
	void initVariables();

public:
	Pad();

	sf::RectangleShape getRect();
};