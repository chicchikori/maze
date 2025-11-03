#pragma once

#include "../../Files/Settings.h"
#include "../ControlMaze/Control.h"
#include "../Util/rc.h"
#include "../Util/uf.h"
#include "Line.h"
#include "UnionFind.h"

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <array>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace rc
{
    class Road : public Uf::UnionFind
    {
    public:
        Road() = default;
        explicit Road(const std::shared_ptr<Settings>& setting, const std::shared_ptr<Window>& window);

        void RenderAnswer() const;

        void SetMode(RENDERMODE modes);

        std::unordered_set<std::shared_ptr<Uf::Tree>> GetTree() const
        {
            return trees;
        }

        void UpdateLines();

        void Recenter(Cont::Control& controlers);

        void Render();

        void Update();

        void CreateUf();

        std::unordered_map<sf::Vector2i, std::shared_ptr<Point>, Vec2Hash> GetBoxes() const
        {
            return boxes;
        }

        std::vector<sf::Vector2f> GetAnswer() const
        {
            return line.GetAnswerRoute();
        }

        void Exit();

    private:
        void AddArrow();
        void Init();
        void AddRoad(sf::Vector2f pos1, const std::shared_ptr<rc::Point>& box);


        std::promise<std::unordered_set<std::shared_ptr<Uf::Tree>>> promiseTree;

        std::shared_ptr<Settings> settings;
        std::shared_ptr<Window> window;
        Line line;
        RENDERMODE mode = RENDERMODE::ROAD;
        std::unordered_set<std::shared_ptr<Uf::Tree>> trees;
        std::unordered_map<sf::Vector2i, std::shared_ptr<Point>, Vec2Hash> boxes;

        sf::VertexArray roadVArray = sf::VertexArray(sf::PrimitiveType::Lines);

        std::array<sf::ConvexShape, 2> arrows = {sf::ConvexShape(7), sf::ConvexShape(7)};
        std::array<sf::Vector2i, 7> arrowPos = {sf::Vector2i(0, 0),    sf::Vector2i(35, 0),    sf::Vector2i(0, 20),
                                                sf::Vector2i(35, -30), sf::Vector2i(-35, -30), sf::Vector2i(0, 20),
                                                sf::Vector2i(-35, 0)};

        std::array<sf::Vector2f, 2> sGPos;

        std::array<sf::Color, 2> sGColor = {
            GREEN_COLOR,
            RED_COLOR,
        };
        bool wasCreated{false};
    };
} // namespace rc
