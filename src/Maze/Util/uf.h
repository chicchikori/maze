#pragma once
#include <SFML/System/Vector2.hpp>
#include <memory>

namespace Uf
{
    using ID = int;

    struct Node
    {
        ID id;
        sf::Vector2i posision;
        int size = 1;
        std::shared_ptr<Node> root;
        std::shared_ptr<Node> parent;
    };

    struct Tree
    {
        std::shared_ptr<Node> from;
        std::shared_ptr<Node> to;
        int weight;
        bool isHorizen;
    };

    struct TreeHash
    {
        size_t operator()(const std::shared_ptr<Tree>& tree) const noexcept
        {
            return std::hash<void*>()(tree->from.get()) ^ (std::hash<void*>()(tree->to.get()) << 1);
        }
    };
} // namespace Uf
