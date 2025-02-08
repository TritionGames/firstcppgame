#pragma once

#include "SFML/Graphics.hpp"
#include "Wall.h"
#include <iostream>

class Player {
private:
	sf::RenderWindow* window;
	sf::RectangleShape rect;
	sf::RectangleShape* colliding_tile_with;

	bool colliding = false;

	sf::Texture tile_outline_texture;
	sf::Sprite tile_outline_sprite;

	sf::Font font;
	sf::Text text;

	sf::Texture item_texture;
	sf::Sprite item_sprite;

	sf::Texture texture;
	sf::Sprite sprite;
	
	sf::Vector2f velocity;
	std::vector<sf::Vector2f> positions;
	sf::Vector2i chunk_coord;

	std::vector<std::map<int, int>> inventory;

	void initVariables();
	void setCollidingTile(sf::RectangleShape wall);

	sf::Vector2f origin;// = getPosition();
	sf::Vector2f direction;// = sf::Vector2f(1, 1);
	sf::Vector2i mouse_pos;

	sf::Vector2f max;// = wall_rect.getPosition() + sf::Vector2f(0, wall_rect.getSize().y);
	sf::Vector2f min;// = wall_rect.getPosition() + sf::Vector2f(wall_rect.getSize().x, 0);

	sf::Vector2f intersection;

	std::string removal_position = "None";
	std::string removal_tile;

	std::map<int, float> tile_mining_duration;

	int removal_tile_type;

	float mine_timer = 0;

	bool can_mine = true;
	bool can_jump = false;


public:
	Player();
	virtual ~Player();

	void render(sf::Vector2f camera_pos, std::map<int, sf::Texture> image_id);
	void update(std::map<std::string, std::map<std::string, int>> &chunks, float& dt, sf::Mouse& mouse_pos, sf::RenderWindow &window);
	void setRenderWindow(sf::RenderWindow* window);
	void setFont(sf::Font& font);
	void setVelocity(sf::Vector2f vel);
	sf::Vector2f getPosition() const;
	void get_colliding_walls(std::map<std::string, std::map<std::string, int>> &chunks, std::vector<sf::Vector2f> positions);
	void get_looking_at(std::map<std::string, std::map<std::string, int>>& chunks, std::vector<sf::Vector2f> positions);
	sf::Vector2f getVelocity() const;
};
