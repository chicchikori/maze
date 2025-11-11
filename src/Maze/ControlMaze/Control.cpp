#include "Control.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>
#include <utility>

Cont::Control::Control(const std::shared_ptr<Window>& window,
                       const std::unordered_set<std::shared_ptr<Uf::Tree>>& trees, const MazePeference& preferences) :
    preferences(preferences), window(window)
{
    SetTree(trees);
}
Cont::Control::Control(const std::shared_ptr<Window>& window) :
    window(window) {

    };

void Cont::Control::SetTree(const std::unordered_set<std::shared_ptr<Uf::Tree>>& tree)
{
    std::unordered_set<std::pair<sf::Vector2i, sf::Vector2i>, Vec2PairHash, Vec2PairEqual> treeSet;
    for (const auto& treeEdge : tree)
    {
        treeSet.insert(std::pair<sf::Vector2i, sf::Vector2i>(treeEdge->from->posision, treeEdge->to->posision));
    }
    this->tree = treeSet;
    Init();
}

void Cont::Control::Init()
{
    isGoal = false;
    currentPos = {0, 0};
    route.clear();
    route.emplace_back(0, 0);
    controlLine.clear();
    AddLine(sf::Vector2f(0, 0), sf::Vector2f(-0.5F, 0));
}


void Cont::Control::Event(sf::Event& event )
{
    auto key = (event.getIf<sf::Event::KeyPressed>());
    switch (key->code)
    {
    case sf::Keyboard::Key::Left:
        Move({-1, 0});
        break;
    case sf::Keyboard::Key::Right:
        Move({1, 0});
        break;
    case sf::Keyboard::Key::Up:
        Move({0, -1});
        break;
    case sf::Keyboard::Key::Down:
        Move({0, 1});
        break;
    default:
        break;
    }
}

void Cont::Control::Move(sf::Vector2i delta)
{
    if (isGoal)
    {
        return;
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
    isGoal = currentPos == preferences.GetGoalPos();
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

void Cont::Control::AddLine(const sf::Vector2f pos1, const sf::Vector2f pos2 )
{
    const sf::Vertex vectorFrom{
        .position = NormalToCenterWindowSize(pos1, preferences.GetOffset(), preferences.GetSize()),
        .color = GREEN_COLOR};
    const sf::Vertex vectorTo{
        .position = NormalToCenterWindowSize(pos2, preferences.GetOffset(), preferences.GetSize()),
        .color = GREEN_COLOR};
    controlLine.append(vectorFrom);
    controlLine.append(vectorTo);
}

void Cont::Control::Render() const
{
    window->GetWindow()->draw(controlLine);
}
