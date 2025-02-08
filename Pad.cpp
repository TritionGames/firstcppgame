#include "Pad.h"
#include "SFML/Graphics.hpp"

void Pad::initVariables()
{
	this->rect.setPosition(this->pos);
	this->rect.setFillColor(sf::Color::Blue);
	this->rect.setSize(sf::Vector2f(50.f, 50.f));
};

sf::RectangleShape Pad::getRect() {
	return this->rect;
}

Pad::Pad() {
	this->initVariables();
};