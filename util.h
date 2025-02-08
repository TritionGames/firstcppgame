#pragma once

#include <vector>
#include <string>
#include <sstream>
#include "SFML/Graphics.hpp"

struct Ray;
// Structure to represent an axis-aligned bounding box (AABB)
struct AABB;

std::vector<std::string> adv_tokenizer(const std::string& str, char delimiter);
sf::Vector2f lerp2f(sf::Vector2f a, sf::Vector2f b, float t);
std::string vec2_to_string(sf::Vector2f vec2);
sf::Vector2f string_to_vec2(std::string str, sf::Vector2f scale);
bool rayIntersectsAABB(sf::Vector2f &origin, sf::Vector2f &direction, sf::Vector2f &min, sf::Vector2f &max, float tMin, sf::Vector2f& intersectionPoint);
float distance(sf::Vector2f point1, sf::Vector2f point2);
