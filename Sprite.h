#pragma once

#include "SFML/Graphics.hpp"
#include <iostream>

class Sprite {
private:
	std::string imageName;
	sf::RenderWindow* &window;
	sf::Texture texture;

public:
	Sprite(std::string image_name, sf::RenderWindow target_window) {
		std::string imageName = image_name;
		sf::RenderWindow *window = &target_window;
	};

	void loadSprite(std::string& imageName);
	void render();
	void initVariables();
};