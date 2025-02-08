#include <iostream>
#include "SFML/Graphics.hpp"
#include "util.h"

#include <vector>
#include <string>
#include <sstream>

struct Ray {
    sf::Vector2f origin;
    sf::Vector2f direction; // Should be normalized
};

// Structure to represent an axis-aligned bounding box (AABB)
struct AABB {
    sf::Vector2f min; // Bottom-left corner
    sf::Vector2f max; // Top-right corner
};

std::vector<std::string> adv_tokenizer(const std::string& str, char delimiter)
{
    std::vector<std::string> result;
    std::string token;
    std::istringstream tokenStream(str);

    while (std::getline(tokenStream, token, delimiter)) {
        result.push_back(token);
    }

    return result;
}

sf::Vector2f lerp2f(sf::Vector2f a, sf::Vector2f b, float t) {
    return a + (b - a) * t;
}

std::string vec2_to_string(sf::Vector2f vec2) {
    return std::to_string(int(vec2.x)) + ";" + std::to_string(int(vec2.y));
}

sf::Vector2f string_to_vec2(std::string str, sf::Vector2f scale = sf::Vector2f(1, 1)) {
    std::vector<std::string> vec = adv_tokenizer(str, ';');
    return sf::Vector2f(int(atoi(vec[0].c_str()) * scale.x), int(atoi(vec[1].c_str()) * scale.y));
}

bool rayIntersectsAABB(sf::Vector2f &origin, sf::Vector2f &direction, sf::Vector2f& min, sf::Vector2f &max, float tMin, sf::Vector2f& intersectionPoint) {
    float t1 = (min.x - origin.x) / direction.x;
    float t2 = (max.x - origin.x) / direction.x;
    float t3 = (min.y - origin.y) / direction.y;
    float t4 = (max.y - origin.y) / direction.y;

    // Find the entry and exit points along each axis
    float tMinX = std::min(t1, t2);
    float tMaxX = std::max(t1, t2);
    float tMinY = std::min(t3, t4);
    float tMaxY = std::max(t3, t4);

    // Overall entry and exit points
    tMin = std::max(tMinX, tMinY);
    float tMax = std::min(tMaxX, tMaxY);

    // If tMax < 0, the intersection is behind the ray origin
    if (tMax < 0) return false;

    // If tMin > tMax, the ray misses the rectangle
    if (tMin > tMax) return false;

    // Compute intersection point
    intersectionPoint.x = origin.x + tMin * direction.x;
    intersectionPoint.y = origin.y + tMin * direction.y;

    return true;
}

float distance(sf::Vector2f point1, sf::Vector2f point2) {
    return std::sqrt(std::pow(point1.y - point2.y, 2) + std::pow(point1.x - point2.x, 2));
};

