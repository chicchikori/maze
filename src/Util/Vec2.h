#pragma once

#include <SFML/System/Vector2.hpp>
#include <cstddef>
#include <utility>
struct Vec2Hash {
    std::size_t operator()(const sf::Vector2i& v) const {
        return (static_cast<size_t>(v.x) << 32) ^ static_cast<size_t>(v.y);
    }
};
struct Vec2PairHash {
    std::size_t operator()(
        const std::pair<sf::Vector2i, sf::Vector2i>& v) const {
        Vec2Hash h;
        size_t h1 = h(v.first);
        size_t h2 = h(v.second);
        return h1 ^ h2;
    }
};

struct Vec2PairEqual {
    bool operator()(
        const std::pair<sf::Vector2i, sf::Vector2i>& a,
        const std::pair<sf::Vector2i, sf::Vector2i>& b) const noexcept {
        return (a.first == b.first && a.second == b.second) ||
               (a.first == b.second && a.second == b.first);
    }
};