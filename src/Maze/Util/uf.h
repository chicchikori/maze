#pragma once
#include <SFML/System/Vector2.hpp>
#include <memory>

namespace Uf
{
    using ID = int;

    struct Point
    {
        ID id;
        sf::Vector2i posision;
        int size = 1;
        std::shared_ptr<Point> root;
    };

    struct Edge
    {
        std::shared_ptr<Point> from;
        std::shared_ptr<Point> to;
        int weight;
        bool isHorizen;
    };

    struct EdgeHash
    {
        size_t operator()(const std::shared_ptr<Edge>& edge) const noexcept
        {
            return std::hash<void*>()(edge->from.get()) ^ (std::hash<void*>()(edge->to.get()) << 1);
        }
    };
} // namespace Uf
