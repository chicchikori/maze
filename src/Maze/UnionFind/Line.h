#pragma once

#include "../../Files/Settings.h"
#include "../Util/uf.h"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>
#include <unordered_set>
#include <vector>

class Line
{
public:
    explicit Line(const std::shared_ptr<Settings>& setting, const std::shared_ptr<Window>& window) :
        settings(setting), window(window) {};

    void UpdateLine(const std::unordered_set<std::shared_ptr<Uf::Edge>>& trees);
    void Render(bool showAnswer) const;
    [[nodiscard]] std::vector<sf::Vector2f> SolveMaze(const std::unordered_set<std::shared_ptr<Uf::Edge>>& edges,
                                                      sf::Vector2i start, sf::Vector2i goal) const;

    [[nodiscard]] std::vector<sf::Vector2f> GetAnswerRoute() const
    {
        return answerRoute;
    };

private:
    std::shared_ptr<Settings> settings;
    std::shared_ptr<Window> window;
    sf::VertexArray linesVArray = sf::VertexArray(sf::PrimitiveType::Lines);
    sf::VertexArray answerLine = sf::VertexArray(sf::PrimitiveType::Lines);
    std::vector<sf::Vector2f> answerRoute;
    void AddAnswerLine(int index);
    void AddMazeLine(sf::Vector2f from, sf::Vector2f to);
    sf::Vector2f AddLine(sf::Vector2f vec2);
};
