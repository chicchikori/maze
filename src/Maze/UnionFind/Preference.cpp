#include "Preference.h"
#include <random>
#include "../../Game.h"
#include "../../Util/Variables.h"

void MazePeference::Center()
{
    const auto windowMazeSize = mazeSize + sf::Vector2u(2, 2);
    const sf::Vector2f windowMazeUSize(
        static_cast<float>(Game::window->GetDisplaySize().x) / static_cast<float>(windowMazeSize.x),
        static_cast<float>(Game::window->GetDisplaySize().y) / static_cast<float>(windowMazeSize.y));

    if (windowMazeUSize.x > windowMazeUSize.y)
    {
        size = windowMazeUSize.y;
        offset = sf::Vector2f((static_cast<float>(Game::window->GetWindow()->getSize().x) -
                               (static_cast<float>(windowMazeSize.x) - 2) * size) /
                                  2.F,
                              size + MENU_BAR_HEIGHT);
    }
    else
    {
        size = windowMazeUSize.x;
        offset = sf::Vector2f(size,
                              (static_cast<float>(Game::window->GetWindow()->getSize().y) -
                               (static_cast<float>(windowMazeSize.y) - 2) * size) /
                                  2.F);
    }
}

void MazePeference::UpdateSeed()
{
    if (isUpdateSeed)
    {
        std::random_device randD;
        std::mt19937 gen(randD());
        std::uniform_int_distribution dis(0, INT_MAX);
        seeds = dis(gen);
    }
}

void MazePeference::SetDefault()
{
    seeds = 0;
    isUpdateSeed = true;
    SetMazeSize(DEFAULT_MAZESIZE);
}
