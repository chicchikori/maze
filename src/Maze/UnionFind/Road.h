#pragma once

#include "../../Files/Settings.h"
#include "../ControlMaze/Control.h"
#include "../Util/rc.h"
#include "../Util/uf.h"
#include "Line.h"

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <array>
#include <memory>
#include <unordered_set>
#include <vector>

namespace rc {
class Road {
public:
    explicit Road(const std::shared_ptr<Settings>& setting,
                  const std::shared_ptr<Window>& window);

    void renderAnswer() const;

    void SetMode(RENDERMODE modes);

    void AddEdge(const std::shared_ptr<Uf::Edge>& edge);

    std::unordered_set<std::shared_ptr<Uf::Edge>> GetTree() const {
        return trees;
    }

    void UpdateLines();

    void Recenter(Cont::Control& controlers, bool isGoal);

    void render();

    std::vector<std::shared_ptr<Point>> GetBoxes() const {
        return boxes;
    }

    std::vector<sf::Vector2f> GetAnswer() const {
        return line.GetAnswerRoute();
    }

private:
    void DrawArrow();
    void Init();

private:
    std::shared_ptr<Settings> settings;
    std::shared_ptr<Window> window;
    Line line;
    RENDERMODE mode = RENDERMODE::ROAD;
    std::unordered_set<std::shared_ptr<Uf::Edge>> trees;
    std::vector<std::shared_ptr<Point>> boxes;

    sf::VertexArray RoadVArray = sf::VertexArray(sf::PrimitiveType::Lines);

    std::array<sf::ConvexShape, 2> Arrows = {sf::ConvexShape(7),
                                             sf::ConvexShape(7)};
    const std::array<sf::Vector2i, 7> arrowPos = {
        sf::Vector2i(0, 0), sf::Vector2i(35, 0), sf::Vector2i(0, 20),
        sf::Vector2i(35, -30), sf::Vector2i(-35, -30), sf::Vector2i(0, 20),
        sf::Vector2i(-35, 0)};

    std::array<sf::Vector2f, 2> SGPos;

    const std::array<sf::Color, 2> SGColor = {
        settings->GetGreen(),
        settings->GetRed(),
    };
};
} // namespace rc