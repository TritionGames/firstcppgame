#pragma once

#include "SFML/Graphics.hpp"
#include "Player.h"
#include "Wall.h"
#include <list>

class Game
{
private:
	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::Event ev;

	sf::Text text;
	sf::Font font;

	Player player;

	sf::Texture background_texture;
	sf::Sprite background_sprite;
	std::vector<sf::Vector2f> render_chunks;
	sf::Mouse mouse;
	sf::Vector2f chunk_coord;

	sf::Clock clock;
	float last_time = 0.f;

	std::map<std::string, std::map<std::string, int>> chunks;

	float dt = 0.f;

	sf::Vector2f camera_pos;
	std::vector<sf::Vector2f> positions;

	Wall wall;

	void initVariables();
	void initWindow();

public:
	Game();
	virtual ~Game();

	void render();
	const bool running() const;
	void pollEvents();
	void update();
	void generate_chunk(std::string position, sf::Vector2f chunk);

	int tile_count = 0;
};
