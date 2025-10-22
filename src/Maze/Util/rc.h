#pragma once
#include <SFML/System/Vector2.hpp>
#include <utility>
namespace rc {

using bID = int;

using Wall = std::pair<bID, bID>;

struct Point {
    explicit Point(const sf::Vector2i position) {
        this->position = position;
    }
    sf::Vector2i position;
    bool isLeft = false;
    bool isTop = false;
};

enum class RENDERMODE { ROAD, LINE, BOTH };
}  // namespace rc