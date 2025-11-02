#include "PrintMaze.h"

#include "tinyfiledialogs.h"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include "../Util/Variables.h"

PrintMaze::PrintMaze(const std::shared_ptr<Settings>& setting, const std::vector<std::shared_ptr<rc::Point>>& boxes,
                     const std::vector<sf::Vector2f>& answerRoute) : settings(setting), box(boxes), answer(answerRoute)
{
}

void PrintMaze::AddInfo(const std::vector<std::shared_ptr<rc::Point>>& boxes,
                        const std::vector<sf::Vector2f>& answerRoute)
{
    box = boxes;
    answer = answerRoute;
}

void PrintMaze::Create()
{
    mazeImage =
        sf::Image((settings->GetMazeSize() + sf::Vector2u(2, 2)) * static_cast<unsigned>(IMAGE_ROAD_SIZE), WHITE_COLOR);
    for (const auto& boxes : box)
    {
        if (!(boxes->IsTop()))
        {
            const sf::Vector2u posFrom((boxes->GetPosition() + sf::Vector2i(1, 1)) * IMAGE_ROAD_SIZE);
            const sf::Vector2u posTo((boxes->GetPosition() + sf::Vector2i(2, 1)) * IMAGE_ROAD_SIZE);
            AddLine(posFrom, posTo);
        }
        if (!(boxes->IsLeft()))
        {
            const sf::Vector2u posFrom((boxes->GetPosition() + sf::Vector2i(1, 1)) * IMAGE_ROAD_SIZE);
            const sf::Vector2u posTo((boxes->GetPosition() + sf::Vector2i(1, 2)) * IMAGE_ROAD_SIZE);
            AddLine(posFrom, posTo);
        }
    }
    answerImage = sf::Image(mazeImage);
    for (int i = 0; i < answer.size() - 1; i++)
    {
        const sf::Vector2u vectorFrom{
            sf::Vector2u(static_cast<unsigned>(IMAGE_ROAD_SIZE >> 1), static_cast<unsigned>(IMAGE_ROAD_SIZE >> 1)) +
            static_cast<sf::Vector2u>((answer[i] + sf::Vector2f(1, 1)) * static_cast<float>(IMAGE_ROAD_SIZE))};
        const sf::Vector2u vectorTo{
            sf::Vector2u(static_cast<unsigned>(IMAGE_ROAD_SIZE >> 1), static_cast<unsigned>(IMAGE_ROAD_SIZE >> 1)) +
            static_cast<sf::Vector2u>((answer[i + 1] + sf::Vector2f(1, 1)) * static_cast<float>(IMAGE_ROAD_SIZE))};
        AddLine(vectorFrom, vectorTo, true);
    }
}

bool PrintMaze::Save()
{
    Create();
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

void PrintMaze::AddLine(const sf::Vector2u posFrom, const sf::Vector2u posTo, const bool isAnswer)
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

    auto& image = isAnswer ? answerImage : mazeImage;
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
