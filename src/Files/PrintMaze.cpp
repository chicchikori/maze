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
#define IMAGE_ROAD_SIZE 15
#define IMAGE_WALL_SIZE 1

PrintMaze::PrintMaze(const std::shared_ptr<Settings>& setting,
                     const std::vector<std::shared_ptr<rc::Point>>& boxes,
                     const std::vector<sf::Vector2f>& answerRoute) {
    settings = setting;
    box = boxes;
    answer = answerRoute;
}

void PrintMaze::AddInfo(const std::vector<std::shared_ptr<rc::Point>>& boxes,
                        const std::vector<sf::Vector2f>& answerRoute) {
    box = boxes;
    answer = answerRoute;
}

void PrintMaze::Create() {
    mazeImage = sf::Image((settings->GetMazeSize() + sf::Vector2u(2, 2)) *
                          static_cast<unsigned>(IMAGE_ROAD_SIZE),
                          settings->GetWhite());
    for (const auto& boxes : box) {
        if (!(boxes->isTop)) {
            const sf::Vector2u p1((boxes->position + sf::Vector2i(1, 1)) *
                                  IMAGE_ROAD_SIZE);
            const sf::Vector2u p2((boxes->position + sf::Vector2i(2, 1)) *
                                  IMAGE_ROAD_SIZE);
            AddLine(p1, p2);
        }
        if (!(boxes->isLeft)) {
            const sf::Vector2u p1((boxes->position + sf::Vector2i(1, 1)) *
                                  IMAGE_ROAD_SIZE);
            const sf::Vector2u p2((boxes->position + sf::Vector2i(1, 2)) *
                                  IMAGE_ROAD_SIZE);
            AddLine(p1, p2);
        }
    }
    answerImage = sf::Image(mazeImage);
    for (int i = 0; i < answer.size() - 1; i++) {
        const sf::Vector2u v1{
            sf::Vector2u(static_cast<unsigned>(IMAGE_ROAD_SIZE / 2.f),
                         static_cast<unsigned>(IMAGE_ROAD_SIZE / 2.f)) +
            static_cast<sf::Vector2u>((answer[i] + sf::Vector2f(1, 1)) *
                                      static_cast<float>(IMAGE_ROAD_SIZE))};
        const sf::Vector2u v2{
            sf::Vector2u(static_cast<unsigned>(IMAGE_ROAD_SIZE / 2.f),
                         static_cast<unsigned>(IMAGE_ROAD_SIZE / 2.f)) +
            static_cast<sf::Vector2u>(
                (answer[i + 1] + sf::Vector2f(1, 1)) *
                static_cast<float>(IMAGE_ROAD_SIZE))};
        AddLine(v1, v2, true);
    }
}

bool PrintMaze::Save() {
    Create();
    auto saveDirectory = std::filesystem::path(
        tinyfd_selectFolderDialog(
            "select save folder", ""));
    saveDirectory += "\\maze";
    const auto mazePath = saveDirectory.replace_filename("maze.png");
    const bool isSuccessMaze = mazeImage.saveToFile(mazePath);
    if (!isSuccessMaze) {
        std::cout << "unable to save maze\n";
    }

    const auto answerPath = saveDirectory.replace_filename("answer.png");
    const bool isSuccessAnswer = answerImage.saveToFile(answerPath);
    if (!isSuccessAnswer) {
        std::cout << "unable to save answer\n";
    }
    return isSuccessMaze && isSuccessAnswer;
}

void PrintMaze::AddLine(const sf::Vector2u p1, const sf::Vector2u p2,
                        const bool isAnswer) {
    if (p1 == p2) return;

    const auto dir = sf::Vector2i(p2 - p1);
    const int length = p1.x == p2.x ? std::abs(dir.y) : std::abs(dir.x);
    const sf::Vector2i step(
        std::clamp(static_cast<int>(p2.x) - static_cast<int>(p1.x), -1, 1),
        std::clamp(static_cast<int>(p2.y) - static_cast<int>(p1.y), -1, 1));

    const sf::Vector2i crossNormal =
        p1.x == p2.x ? sf::Vector2i(1, 0) : sf::Vector2i(0, 1);

    auto& image = isAnswer ? answerImage : mazeImage;
    const sf::Color color = isAnswer
                                ? settings->GetRed()
                                : settings->GetBlack();

    for (int i = -IMAGE_WALL_SIZE; i <= length + IMAGE_WALL_SIZE; i++) {
        for (int w = -IMAGE_WALL_SIZE; w <= IMAGE_WALL_SIZE; w++) {
            if (sf::Vector2i pos = sf::Vector2i(p1) + step * i + crossNormal * w
                ;
                pos.x >= 0 && pos.y >= 0 && pos.x < static_cast<int>(image.
                    getSize().x) &&
                pos.y < static_cast<int>(image.getSize().y)) {
                image.setPixel(sf::Vector2u(pos), color);
            }
        }
    }
}