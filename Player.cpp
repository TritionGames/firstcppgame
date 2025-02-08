#include "Player.h"
#include "Wall.h"
#include "SFML/Graphics.hpp"
#include "util.h"

#include <iostream>
#include <list>

Player::Player() {
	this->window = nullptr;

	this->initVariables();
}

Player::~Player() {
}

void Player::initVariables() {
	tile_mining_duration[1] = 0.1;
	tile_mining_duration[2] = 0.1;
	tile_mining_duration[3] = 0.2;

	std::map<int, int> dirt;
	dirt.emplace(1, 10);

	std::map<int, int> grass;
	grass.emplace(2, 5);

	inventory.push_back(dirt);
	inventory.push_back(grass);

	

	this->rect.setFillColor(sf::Color::White);
	this->rect.setSize(sf::Vector2f(20.f, 40.f));
	this->rect.setPosition(sf::Vector2f(10000, -800));

	if (!this->texture.loadFromFile("pigeon.png")) {
		//error
	}

	tile_outline_texture.loadFromFile("tile_outline.png");
	tile_outline_sprite.setTexture(tile_outline_texture);

	this->sprite.setTexture(this->texture);
}

void Player::render(sf::Vector2f camera_pos, std::map<int, sf::Texture> image_id) {
	if (removal_position != "None") {
		tile_outline_sprite.setPosition(string_to_vec2(removal_position, sf::Vector2f(200, 200)) + string_to_vec2(removal_tile, sf::Vector2f(50, 50)) - camera_pos);
		window->draw(tile_outline_sprite);
	}

	int count = 0;
	
	for (auto item : inventory) {
		item_sprite.setTexture(image_id[item.begin()->first]);
		item_sprite.setPosition(sf::Vector2f(10 + count*60, 10));
		window->draw(item_sprite);

		text.setPosition(sf::Vector2f(10 + count * 60, 30));
		text.setString(std::to_string(item.begin()->second));
		window->draw(text);
		count++;
	}

	this->sprite.setPosition(rect.getPosition() - camera_pos);
	this->window->draw(sprite);
}

void Player::setCollidingTile(sf::RectangleShape wall) {
	this->colliding_tile_with = &wall;
};

void Player::get_colliding_walls(std::map<std::string, std::map<std::string, int>> &chunks, std::vector<sf::Vector2f> positions) {
	this->colliding = false;

	sf::RectangleShape wall_rect;
	wall_rect.setSize(sf::Vector2f(50, 50));

	sf::Vector2f global_pos;
	std::string z;

	for (auto position : positions) {
		sf::Vector2f gChunk_pos = position;
		gChunk_pos.x *= 200;
		gChunk_pos.y *= 200;

		for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {

			global_pos = sf::Vector2f(x * 50, y * 50) + gChunk_pos;

			z = std::to_string(int(x)) + ";" + std::to_string(int(y));
			
			if (!chunks[vec2_to_string(position)][z]) {
				continue;
			}
			
			wall_rect.setPosition(global_pos);

			if (this->rect.getGlobalBounds().intersects(wall_rect.getGlobalBounds())) {
				this->colliding = true;
				//std::cout << wall_rect.getPosition().x << ", " << wall_rect.getPosition().y << ", " << tile.second << std::endl;
				this->setCollidingTile(wall_rect);
			}
			}

		}
		
	}
}

void Player::get_looking_at(std::map<std::string, std::map<std::string, int>>& chunks, std::vector<sf::Vector2f> positions) {
	this->colliding = false;

	sf::RectangleShape wall_rect;
	wall_rect.setSize(sf::Vector2f(50, 50));

	float min_distance = 150;

	sf::Vector2f global_pos;
	std::string old_removal_chunk = removal_position;
	std::string old_removal_tile = removal_tile;
	std::string z;

	removal_position = "None";

	for (auto position : positions) {
		sf::Vector2f gChunk_pos = position;
		gChunk_pos.x *= 200;
		gChunk_pos.y *= 200;

		for (int x = 0; x < 4; x++) {
			for (int y = 0; y < 4; y++) {

				global_pos = sf::Vector2f(x * 50, y * 50) + gChunk_pos;

				z = std::to_string(int(x)) + ";" + std::to_string(int(y));

				if (!chunks[vec2_to_string(position)][z]) {
					continue;
				}

				wall_rect.setPosition(global_pos);

				origin = getPosition() + sf::Vector2f(10, 20);
				direction = sf::Vector2f(mouse_pos) - sf::Vector2f(400, 300) - sf::Vector2f(10, 20);

				max = wall_rect.getPosition() + sf::Vector2f(0, wall_rect.getSize().y);
				min = wall_rect.getPosition() + sf::Vector2f(wall_rect.getSize().x, 0);

				intersection = sf::Vector2f(0, 0);

				float dist = distance(wall_rect.getPosition(), getPosition());

				if ((dist < min_distance) && rayIntersectsAABB(origin, direction, min, max, 0, intersection)) {
					removal_position = vec2_to_string(position);
					removal_tile = z;
					removal_tile_type = chunks[removal_position][z];

					min_distance = dist;
				}
			}
		}
	}

	if (removal_position != old_removal_chunk || removal_tile != old_removal_tile) {
		mine_timer = 0;
	}

}


void Player::update(std::map<std::string, std::map<std::string, int>> &chunks, float& dt, sf::Mouse &mouse_pos, sf::RenderWindow &window) {
	this->mouse_pos = mouse_pos.getPosition(window);
	this->velocity.x = this->velocity.x / std::pow(10000, dt);
	
	if(this->velocity.y < 1000) this->velocity.y += 1300 * dt;
	//this->velocity.y = this->velocity.y / 4;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		this->velocity.x += 2000 * dt;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		this->velocity.x -= 2000 * dt;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && this->can_jump){
		this->velocity.y = -400;
	}

	this->can_jump = false;

	//sf::Vector2f old_pos = sf::Vector2f(this->rect.getPosition());
	//sf::Vector2f new_pos = this->rect.getPosition() + sf::Vector2f(0, this->velocity.y * dt
	chunk_coord = sf::Vector2i(rect.getPosition());
	chunk_coord.x = chunk_coord.x / 200;
	chunk_coord.y = chunk_coord.y / 200;

	positions = {
		sf::Vector2f(int(chunk_coord.x - 1), int(chunk_coord.y - 1)),
		sf::Vector2f(int(chunk_coord.x), int(chunk_coord.y - 1)),
		sf::Vector2f(int(chunk_coord.x + 1), int(chunk_coord.y - 1)),
		sf::Vector2f(int(chunk_coord.x - 1), int(chunk_coord.y)),
		sf::Vector2f(int(chunk_coord.x), int(chunk_coord.y)),
		sf::Vector2f(int(chunk_coord.x + 1), int(chunk_coord.y)),
		sf::Vector2f(int(chunk_coord.x - 1), int(chunk_coord.y + 1)),
		sf::Vector2f(int(chunk_coord.x), int(chunk_coord.y + 1)),
		sf::Vector2f(int(chunk_coord.x + 1), int(chunk_coord.y + 1)),
	};


	this->get_looking_at(chunks, positions);

	if (removal_position != "None" && can_mine) {
		if (mouse_pos.isButtonPressed(sf::Mouse::Left)) {
			mine_timer += dt;

			if (mine_timer > tile_mining_duration[removal_tile_type]) {
				mine_timer = 0;
				chunks[removal_position][removal_tile] = 0;

				removal_position = "None";
			}
		}
		else {
			mine_timer = 0;
		}
	}

	this->rect.setPosition((this->rect.getPosition() + sf::Vector2f(0, this->velocity.y * dt)));
	this->get_colliding_walls(chunks, positions);

	if (this->colliding) {

		if (this->velocity.y > 0) {
			this->velocity.y = 0;
			this->can_jump = true;
			this->rect.setPosition(sf::Vector2f(this->rect.getPosition().x, this->colliding_tile_with->getPosition().y - this->rect.getSize().y));
		}
		else if (this->velocity.y < 0) {
			this->velocity.y = 0;
			this->rect.setPosition(sf::Vector2f(this->rect.getPosition().x, this->colliding_tile_with->getPosition().y + this->colliding_tile_with->getSize().y));
		}
	}

	this->rect.setPosition((this->rect.getPosition() + sf::Vector2f(this->velocity.x * dt, 0)));

	this->get_colliding_walls(chunks, positions);

	if (this->colliding) {

		if (this->velocity.x > 0) {
			this->velocity.x = 0;
			this->rect.setPosition(sf::Vector2f(this->colliding_tile_with->getPosition().x - this->rect.getSize().x, this->rect.getPosition().y));
		}
		else if (this->velocity.x < 0) {
			this->velocity.x = 0;
			this->rect.setPosition(sf::Vector2f(this->colliding_tile_with->getPosition().x + this->colliding_tile_with->getSize().x, this->rect.getPosition().y));
		}
	}

	//std::cout << std::to_string(this->rect.getPosition()) << std::endl;
	//std::cout << this->rect.getPosition().y << std::endl;
	
}

void Player::setRenderWindow(sf::RenderWindow* window) {
	this->window = window;

}

void Player::setFont(sf::Font& font) {
	this->font = font;
	this->text.setFont(this->font);
}

void Player::setVelocity(sf::Vector2f vel) {
	this->velocity = vel;
}

sf::Vector2f Player::getVelocity() const{
	return this->velocity;
}

sf::Vector2f Player::getPosition() const {
	return this->rect.getPosition();
}
