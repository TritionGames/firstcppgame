#pragma once

#include "SFML/Graphics.hpp"

class Wall {
private:
	
	sf::RectangleShape rect;
	sf::RenderWindow* window;

	sf::Texture texture;
	sf::Texture texture2;
	sf::Sprite sprite;

	std::map<int, sf::Texture> image_id;
	std::map<int, std::string> file_name_id;

	int block_type = 1;
	void initVariables();

public:
	Wall();
	void set_block_type(int &block_type);
	void render(sf::Vector2f camera_pos);
	sf::RectangleShape getBounds() const;
	std::map<int, sf::Texture> get_images() const;
	void setRenderWindow(sf::RenderWindow* window);
	void setPosition(sf::Vector2f pos);
};