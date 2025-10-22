#pragma once
#include "../../Files/Settings.h"
#include "../../Util/Vec2.h"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>
#include <unordered_set>
#include <utility>
#include <vector>

namespace Cont {
enum class MoveDir : int { Up = 1, Down = -1, Right = 2, Left = -2 };

class Control {
public:
    Control() = default;
    explicit Control(const std::shared_ptr<Settings>& setting,
                     const std::shared_ptr<Window>& window);
    Control(const std::shared_ptr<Settings>& setting,
            const std::shared_ptr<Window>& window,
            const std::unordered_set<std::pair<sf::Vector2i, sf::Vector2i>,
                                     Vec2PairHash, Vec2PairEqual>& tree);
    void Render() const;
    void Move(MoveDir direction);
    void Update();

private:
    void AddLine(sf::Vector2f pos1, sf::Vector2f pos2);
    std::shared_ptr<Settings> settings;
    std::shared_ptr<Window> window;
    sf::Vector2i currentPos;
    std::vector<sf::Vector2i> route;
    sf::VertexArray controlLine = sf::VertexArray(sf::PrimitiveType::Lines);
    std::unordered_set<std::pair<sf::Vector2i, sf::Vector2i>, Vec2PairHash,
                       Vec2PairEqual>
    tree;
    bool isGoal{false};
};
} // namespace Cont