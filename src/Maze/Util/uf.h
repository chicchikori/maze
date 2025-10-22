#pragma once
#include <SFML/System/Vector2.hpp>
#include <memory>

namespace Uf {
using ID = int;

struct Point {
    Point(const ID id, const sf::Vector2i posision) {
        this->id = id;
        this->posision = posision;
    }

    ID id;
    sf::Vector2i posision;
    int size = 1;
    std::shared_ptr<Point> root;
};

struct Edge {
    Edge(const std::shared_ptr<Point>& from, const std::shared_ptr<Point>& to,
         int weight, bool isHorizon);
    std::shared_ptr<Point> from;
    std::shared_ptr<Point> to;
    int weight;
    bool isHorizen;

    bool operator==(const Edge& other) const {
        return from == other.from && to == other.to;
    }
};

struct EdgeHash {
    size_t operator()(const std::shared_ptr<Edge>& e) const noexcept {
        return std::hash<void*>()(e->from.get()) ^
               (std::hash<void*>()(e->to.get()) << 1);
    }
};
} // namespace uf