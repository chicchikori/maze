#include "PrintMaze.h"

#include "tinyfiledialogs.h"

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include "../Maze/UnionFind/Road.h"
#include "../Util/Variables.h"


sf::Image PrintMaze::DrawMazeImage(const MazePeference& pref,
                                   const std::unordered_map<sf::Vector2i, std::shared_ptr<rc::Point>, Vec2Hash>& box)
{
    auto mazeImage =
        sf::Image((pref.GetMazeSize() + sf::Vector2u(2, 2)) * static_cast<unsigned>(IMAGE_ROAD_SIZE), WHITE_COLOR);
    for (const auto& boxes : box)
    {
        if (!(boxes.second->IsTop()))
        {
            const sf::Vector2u posFrom((boxes.second->GetPosition() + sf::Vector2i(1, 1)) * IMAGE_ROAD_SIZE);
            const sf::Vector2u posTo((boxes.second->GetPosition() + sf::Vector2i(2, 1)) * IMAGE_ROAD_SIZE);
            AddLine(posFrom, posTo, mazeImage);
        }
        if (!(boxes.second->IsLeft()))
        {
            const sf::Vector2u posFrom((boxes.second->GetPosition() + sf::Vector2i(1, 1)) * IMAGE_ROAD_SIZE);
            const sf::Vector2u posTo((boxes.second->GetPosition() + sf::Vector2i(1, 2)) * IMAGE_ROAD_SIZE);
            AddLine(posFrom, posTo, mazeImage);
        }
    }
    return mazeImage;
}
sf::Image PrintMaze::DrawMazeAnswerImage(const sf::Image& mazeImage, const std::vector<sf::Vector2f>& answer)
{
    auto answerImage = sf::Image(mazeImage);
    for (int i = 0; i < answer.size() - 1; i++)
    {
        const sf::Vector2u vectorFrom{
            sf::Vector2u(static_cast<unsigned>(IMAGE_ROAD_SIZE >> 1), static_cast<unsigned>(IMAGE_ROAD_SIZE >> 1)) +
            static_cast<sf::Vector2u>((answer[i] + sf::Vector2f(1, 1)) * static_cast<float>(IMAGE_ROAD_SIZE))};
        const sf::Vector2u vectorTo{
            sf::Vector2u(static_cast<unsigned>(IMAGE_ROAD_SIZE >> 1), static_cast<unsigned>(IMAGE_ROAD_SIZE >> 1)) +
            static_cast<sf::Vector2u>((answer[i + 1] + sf::Vector2f(1, 1)) * static_cast<float>(IMAGE_ROAD_SIZE))};
        AddLine(vectorFrom, vectorTo, answerImage, true);
    }
    return answerImage;
}

bool PrintMaze::Save(const std::shared_ptr<MazePeference>& pref)
{
    auto road = rc::Road(pref);
    road.CreateNewUf(*pref);

    auto answer = Answer::SolveMaze(road.GetTree(), pref->GetGoalPos());

    sf::Image mazeImage = DrawMazeImage(*pref, road.GetBoxes());
    sf::Image answerImage = DrawMazeAnswerImage(mazeImage, answer);

    auto saveDirectory = std::filesystem::path(tinyfd_selectFolderDialog("select save folder", ""));
    saveDirectory += "\\maze";
    const auto mazePath = saveDirectory.replace_filename("maze.png");
    const bool isSuccessMaze = mazeImage.saveToFile(mazePath);
    if (!isSuccessMaze)
    {
        std::cout << "unable to save maze\n";
    }

    const auto answerPath = saveDirectory.replace_filename("answer.png");
    const bool isSuccessAnswer = answerImage.saveToFile(answerPath);
    if (!isSuccessAnswer)
    {
        std::cout << "unable to save answer\n";
    }
    return isSuccessMaze && isSuccessAnswer;
}

void PrintMaze::AddLine(const sf::Vector2u& posFrom, const sf::Vector2u& posTo, sf::Image& image, const bool& isAnswer)
{
    if (posFrom == posTo)
    {
        return;
    }

    const auto dir = sf::Vector2i(posTo - posFrom);
    const int length = posFrom.x == posTo.x ? std::abs(dir.y) : std::abs(dir.x);
    const sf::Vector2i lineNormal(std::clamp(static_cast<int>(posTo.x) - static_cast<int>(posFrom.x), -1, 1),
                                  std::clamp(static_cast<int>(posTo.y) - static_cast<int>(posFrom.y), -1, 1));

    const sf::Vector2i crossNormal = posFrom.x == posTo.x ? sf::Vector2i(1, 0) : sf::Vector2i(0, 1);

    const sf::Color color = isAnswer ? RED_COLOR : BLACK_COLOR;

    for (int i = -IMAGE_WALL_SIZE; i <= length + IMAGE_WALL_SIZE; i++)
    {
        for (int widthIndex = -IMAGE_WALL_SIZE; widthIndex <= IMAGE_WALL_SIZE; widthIndex++)
        {
            if (sf::Vector2i pos = sf::Vector2i(posFrom) + lineNormal * i + crossNormal * widthIndex; pos.x >= 0 &&
                pos.y >= 0 && pos.x < static_cast<int>(image.getSize().x) &&
                pos.y < static_cast<int>(image.getSize().y))
            {
                image.setPixel(sf::Vector2u(pos), color);
            }
        }
    }
}
