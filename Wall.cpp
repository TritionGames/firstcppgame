#include "Wall.h"
#include "iostream"

Wall::Wall() {
	this->initVariables();
}

void Wall::initVariables() {
	this->rect.setFillColor(sf::Color::Green);
	this->rect.setSize(sf::Vector2f(50.f, 50.f));
	this->rect.setPosition(sf::Vector2f(0.f, 400.f));

	/*if (!this->texture.loadFromFile()) {
		std::cout << "Error loading image" << std::endl;
	}*/
	//this->sprite.setScale(sf::Vector2f(.1, .));

	sf::Texture dirt;
	dirt.loadFromFile("dirt.png");

	sf::Texture grass;
	grass.loadFromFile("grass.png");

	sf::Texture stone;
	stone.loadFromFile("stone.png");

	image_id.emplace(1, dirt);
	image_id.emplace(2, grass);
	image_id.emplace(3, stone);

	this->sprite.setTexture(image_id[block_type]);
}

void Wall::set_block_type(int &id) {
	this->block_type = id;

	this->sprite.setTexture(image_id[block_type]);
}

void Wall::render(sf::Vector2f camera_pos) {
	this->sprite.setPosition(rect.getPosition() - camera_pos);
	//std::cout << this->rect.getGlobalBounds << std::endl;
	//this->window->draw(this->rect);
	this->window->draw(this->sprite);
}

void Wall::setRenderWindow(sf::RenderWindow* window) {
	this->window = window;
}

sf::RectangleShape Wall::getBounds() const{
	return this->rect;
}

void Wall::setPosition(sf::Vector2f pos) {
	this->rect.setPosition(pos);
	this->sprite.setPosition(this->rect.getPosition());
}

std::map<int, sf::Texture> Wall::get_images() const {
	return image_id;
}