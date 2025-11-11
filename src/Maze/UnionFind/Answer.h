#pragma once

#include "../Util/uf.h"
#include "Preference.h"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>
#include <unordered_set>
#include <vector>

class Answer
{
public:
    Answer() = default;

    void UpdateLine(const std::unordered_set<std::shared_ptr<Uf::Tree>>& trees, const MazePeference& preferences);
    void Render() const;
    static std::vector<sf::Vector2f> SolveMaze(const std::unordered_set<std::shared_ptr<Uf::Tree>>& trees,
                                               sf::Vector2i goal);

    [[nodiscard]] std::vector<sf::Vector2f> GetAnswerRoute() const
    {
        return answerRoute;
    };

private:
    sf::VertexArray answerLine = sf::VertexArray(sf::PrimitiveType::Lines);
    std::vector<sf::Vector2f> answerRoute;
    void AddAnswerLine(int index, const MazePeference& preferences);
    sf::Vector2f AddLine(sf::Vector2f vec2);
};
