#pragma once
#include "../../Util/Vec2.h"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>
#include <unordered_set>
#include <utility>
#include <vector>
#include "../Util/uf.h"


#include "../UnionFind/Preference.h"
namespace Cont
{
    enum class MoveDir
    {
        Up,
        Down,
        Right,
        Left
    };

    class Control
    {
    public:
        Control() = default;
        Control(const std::unordered_set<std::shared_ptr<Uf::Tree>>& tree, const MazePeference& pref);
        void SetTree(const std::unordered_set<std::shared_ptr<Uf::Tree>>& tree, const MazePeference& pref);
        void Render() const;
        void Update(const MazePeference& pref);
        void Event(sf::Event& event, const MazePeference& pref);

    private:
        void Init(const MazePeference& pref);
        void Move(sf::Vector2i delta, const MazePeference& pref);
        void AddLine(sf::Vector2f pos1, sf::Vector2f pos2, const MazePeference& pref);
        sf::Vector2i currentPos{-1, -1};
        std::vector<sf::Vector2i> route;
        sf::VertexArray controlLine = sf::VertexArray(sf::PrimitiveType::Lines);
        std::unordered_set<std::pair<sf::Vector2i, sf::Vector2i>, Vec2PairHash, Vec2PairEqual> tree;
        // std::unordered_set<std::shared_ptr<Uf::Tree>> trees;
        sf::Vector2i goalPos{0, 0};
        bool isGoal{false};
    };
} // namespace Cont
