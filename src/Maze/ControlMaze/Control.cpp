#include "Control.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>
#include <utility>

Cont::Control::Control(
    const std::shared_ptr<Settings>& setting, const std::shared_ptr<Window>& window,
    const std::unordered_set<std::pair<sf::Vector2i, sf::Vector2i>, Vec2PairHash, Vec2PairEqual>& trees) :
    settings(setting), window(window), tree(trees)
{
    currentPos = {0, 0};
    route.clear();
    route.emplace_back(0, 0);
    controlLine.clear();
    AddLine(sf::Vector2f(0, 0), sf::Vector2f(-0.5F, 0));
    goalPos = sf::Vector2i((int)settings->GetMazeSize().x - 1, (int)settings->GetMazeSize().y - 1);
}

void Cont::Control::Move(const Cont::MoveDir direction)
{
    if (isGoal)
    {
        return;
    }
    sf::Vector2i delta;
    switch (direction)
    {
        using enum Cont::MoveDir;
    case Right:
        delta = {1, 0};
        break;
    case Left:
        delta = {-1, 0};
        break;
    case Down:
        delta = {0, 1};
        break;
    case Up:
        delta = {0, -1};
        break;
    }

    currentPos += delta;

    if (route.size() > 1 && route.at(route.size() - 2) == currentPos)
    {
        route.pop_back();
    }
    else
    {
        if (tree.contains(std::pair(route.back(), currentPos)))
        {
            route.push_back(currentPos);
        }
        else
        {
            currentPos -= delta;
        }
    }
    isGoal = currentPos == goalPos;
    Update();
}

void Cont::Control::Update()
{
    controlLine.clear();

    AddLine(sf::Vector2f(0, 0), sf::Vector2f(-0.5F, 0));

    for (int i = 0; i < route.size() - 1; i++)
    {
        AddLine(static_cast<sf::Vector2f>(route[i]), static_cast<sf::Vector2f>(route[i + 1]));
    }
    if (isGoal)
    {
        AddLine(static_cast<sf::Vector2f>(route[route.size() - 1]),
                static_cast<sf::Vector2f>(route[route.size() - 1]) + sf::Vector2f(0.5F, 0));
    }
}

void Cont::Control::AddLine(const sf::Vector2f pos1, const sf::Vector2f pos2)
{
    const sf::Vertex vectorFrom{.position = NormalToCenterWindowSize(pos1, settings->GetOffset(), settings->GetSize()),
                                .color = GREEN_COLOR};
    const sf::Vertex vectorTo{.position = NormalToCenterWindowSize(pos2, settings->GetOffset(), settings->GetSize()),
                              .color = GREEN_COLOR};
    controlLine.append(vectorFrom);
    controlLine.append(vectorTo);
}

void Cont::Control::Render() const
{
    window->GetWindow()->draw(controlLine);
}

Cont::Control::Control(const std::shared_ptr<Settings>& setting, const std::shared_ptr<Window>& window) :
    settings(setting), window(window) {

    };
