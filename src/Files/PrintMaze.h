#pragma once

#include "../Files/Settings.h"
#include "../Maze/Util/rc.h"
#include "../Util/Vec2.h"

#include <SFML/Graphics/Image.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>
#include <unordered_map>
#include <vector>

class PrintMaze
{
public:
    PrintMaze() = default;

    explicit PrintMaze(const std::shared_ptr<Settings>& setting) : settings(setting) {};
    PrintMaze(const std::shared_ptr<Settings>& setting,
              const std::unordered_map<sf::Vector2i, std::shared_ptr<rc::Point>, Vec2Hash>& boxes,
              const std::vector<sf::Vector2f>& answerRoute);
    void AddInfo(const std::unordered_map<sf::Vector2i, std::shared_ptr<rc::Point>, Vec2Hash>& boxes,
                 const std::vector<sf::Vector2f>& answerRoute);
    void Create();
    bool Save();

private:
    std::shared_ptr<Settings> settings;
    void AddLine(sf::Vector2u posFrom, sf::Vector2u posTo, bool isAnswer = false);
    sf::Image mazeImage;
    sf::Image answerImage;
    std::unordered_map<sf::Vector2i, std::shared_ptr<rc::Point>, Vec2Hash> box;
    std::vector<sf::Vector2f> answer;
};
