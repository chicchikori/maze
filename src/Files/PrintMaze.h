#pragma once


#include "../Maze/Util/rc.h"
#include "../Util/Vec2.h"


#include <SFML/Graphics/Image.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>
#include <unordered_map>
#include <vector>
#include "../Maze/UnionFind/Preference.h"


class PrintMaze
{
public:
    static bool Save(const MazePeference& pref);

private:
    static sf::Image DrawMazeImage(const MazePeference& pref,
                                   const std::unordered_map<sf::Vector2i, std::shared_ptr<rc::Point>, Vec2Hash>& box);
    static sf::Image DrawMazeAnswerImage(const sf::Image& mazeImage, const std::vector<sf::Vector2f>& answer);
    static void AddLine(const sf::Vector2u& posFrom, const sf::Vector2u& posTo, sf::Image& image,
                        const bool& isAnswer = false);
};
