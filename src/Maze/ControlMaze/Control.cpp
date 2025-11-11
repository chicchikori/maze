#include "Control.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>
#include <utility>
#include "../../Game.h"
#include "../../Util/Variables.h"



Cont::Control::Control(const std::unordered_set<std::shared_ptr<Uf::Tree>>& trees, const MazePeference& pref)
{
    SetTree(trees, pref);
}

void Cont::Control::SetTree(const std::unordered_set<std::shared_ptr<Uf::Tree>>& tree, const MazePeference& pref)
{
    std::unordered_set<std::pair<sf::Vector2i, sf::Vector2i>, Vec2PairHash, Vec2PairEqual> treeSet;
    for (const auto& treeEdge : tree)
    {
        treeSet.insert(std::pair<sf::Vector2i, sf::Vector2i>(treeEdge->from->posision, treeEdge->to->posision));
    }
    this->tree = treeSet;
    Init(pref);
}

void Cont::Control::Init(const MazePeference& pref)
{
    isGoal = false;
    currentPos = {0, 0};
    route.clear();
    route.emplace_back(0, 0);
    controlLine.clear();
    AddLine(sf::Vector2f(0, 0), sf::Vector2f(-0.5F, 0), pref);
}


void Cont::Control::Event(sf::Event& event, const MazePeference& pref)
{
    auto key = (event.getIf<sf::Event::KeyPressed>());
    switch (key->code)
    {
    case sf::Keyboard::Key::Left:
        Move({-1, 0}, pref);
        break;
    case sf::Keyboard::Key::Right:
        Move({1, 0}, pref);
        break;
    case sf::Keyboard::Key::Up:
        Move({0, -1}, pref);
        break;
    case sf::Keyboard::Key::Down:
        Move({0, 1}, pref);
        break;
    default:
        break;
    }
}

void Cont::Control::Move(sf::Vector2i delta, const MazePeference& pref)
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
    isGoal = currentPos == pref.GetGoalPos();
    Update(pref);
}


void Cont::Control::Update(const MazePeference& pref)
{
    controlLine.clear();

    AddLine(sf::Vector2f(0, 0), sf::Vector2f(-0.5F, 0), pref);

    for (int i = 0; i < route.size() - 1; i++)
    {
        AddLine(static_cast<sf::Vector2f>(route[i]), static_cast<sf::Vector2f>(route[i + 1]), pref);
    }
    if (isGoal)
    {
        AddLine(static_cast<sf::Vector2f>(route[route.size() - 1]),
                static_cast<sf::Vector2f>(route[route.size() - 1]) + sf::Vector2f(0.5F, 0), pref);
    }
}

void Cont::Control::AddLine(const sf::Vector2f pos1, const sf::Vector2f pos2, const MazePeference& pref)
{
    const sf::Vertex vectorFrom{.position = NormalToCenterWindowSize(pos1, pref.GetOffset(), pref.GetSize()),
                                .color = GREEN_COLOR};
    const sf::Vertex vectorTo{.position = NormalToCenterWindowSize(pos2, pref.GetOffset(), pref.GetSize()),
                              .color = GREEN_COLOR};
    controlLine.append(vectorFrom);
    controlLine.append(vectorTo);
}

void Cont::Control::Render() const
{
    Game::window.GetWindow()->draw(controlLine);
}
