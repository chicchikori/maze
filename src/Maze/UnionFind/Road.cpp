#include "Road.h"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <cstddef>
#include "../../Game.h"
#include "../Util/uf.h"
#include "Answer.h"
#include "Preference.h"
#include "UnionFind.h"



#include <future>
#include <iostream>
#include <memory>


rc::Road::Road(const std::shared_ptr<MazePeference>& pref) : pref(pref)
{
    Init(*pref);
}

void rc::Road::Init(const MazePeference& pref)
{
    sf::Vector2i goalPos = sf::Vector2i((int)pref.GetMazeSize().x, (int)pref.GetMazeSize().y - 1);
    // pref.Center();
    trees.clear();
    boxes.clear();
    // init box
    boxes.reserve((size_t)(pref.GetMazeSize().x + 1) * (pref.GetMazeSize().y + 1));
    for (int x = 0; x < pref.GetMazeSize().x + 1; x++) // NOLINT
    {
        for (int y = 0; y < pref.GetMazeSize().y + 1; y++) // NOLINT
        {
            auto point = std::make_shared<Point>(sf::Vector2i(x, y));
            if (point->GetPosition().x == pref.GetMazeSize().x)
            {
                point->SetIsTop();
            }
            if (point->GetPosition().y == pref.GetMazeSize().y || point->GetPosition() == sf::Vector2i(0, 0) ||
                point->GetPosition() == goalPos)
            {
                point->SetIsLeft();
            }
            boxes.emplace(point->GetPosition(), point);
        }
    }
}

void rc::Road::UpdateLines(const MazePeference& pref)
{
    line.UpdateLine(trees, pref);
    // wall
    roadVArray.clear();

    for (const auto& box : boxes)
    {
        if (!(box.second->IsTop()))
        {
            AddRoad({1, 0}, box.second);
        }
        if (!(box.second->IsLeft()))
        {
            AddRoad({0, 1}, box.second);
        }
    }

    AddArrow(pref);
}

void rc::Road::CreateUf()
{
    if (!Uf::UnionFind::isRunThread)
    {
        std::cout << "start\n";
        pref->Center();
        pref->UpdateSeed();
        Init(*pref);
        Uf::UnionFind::Create(promiseTree, *pref);
        wasCreated = true;
        std::cout << "end\n";
    }
}

void rc::Road::CreateNewUf(MazePeference& pref)
{
    pref.Center();
    pref.SetSeeds(pref.GetSeeds());
    Init(pref);
    trees = Uf::UnionFind::Create(pref);
    ModifyBoxWalls();
}

void rc::Road::AddRoad(sf::Vector2f pos1, const std::shared_ptr<rc::Point>& box)
{
    sf::Vertex position1{.position = NormalToWindowSize(static_cast<sf::Vector2f>(box->GetPosition()),
                                                        pref->GetOffset(), pref->GetSize()),
                         .color = WHITE_COLOR};
    sf::Vertex position2{.position = NormalToWindowSize(static_cast<sf::Vector2f>(box->GetPosition()) + pos1,
                                                        pref->GetOffset(), pref->GetSize()),
                         .color = WHITE_COLOR};
    roadVArray.append(position1);
    roadVArray.append(position2);
}

void rc::Road::AddArrow(const MazePeference& pref)
{
    float clampedSize = std::clamp(pref.GetSize(), 0.F, 60.F);
    sGPos[0] = pref.GetOffset() + sf::Vector2f(0, 0.5F) * pref.GetSize();
    sGPos[1] = pref.GetOffset() + ((sf::Vector2f)(pref.GetMazeSize()) + sf::Vector2f(0, -0.5F)) * pref.GetSize();
    for (int j = 0; j < 2; j++)
    {
        sf::Vector2i pos((j - 1) * 100 + 10, 10);
        for (int i = 0; i < 7; i++)
        {
            pos += arrowPos.at(i);
            arrows.at(j).setPoint(i, (sf::Vector2f)pos / 100.F * clampedSize);
            arrows.at(j).setPosition(sGPos.at(j));
            arrows.at(j).setFillColor(sGColor.at(j));
        }
    }
}
void rc::Road::Update()
{
    if (!Uf::UnionFind::isRunThread && wasCreated)
    {
        trees = promiseTree.get_future().get();
        promiseTree = std::promise<std::unordered_set<std::shared_ptr<Uf::Tree>>>();
        wasCreated = false;
        std::cout << "got\n";
        ModifyBoxWalls();
    }
}

void rc::Road::ModifyBoxWalls()
{
    for (auto& tree : trees)
    {
        auto& point = boxes.at(tree->to->posision);
        if (tree->isHorizen)
        {
            point->SetIsLeft();
        }
        else
        {
            point->SetIsTop();
        }
    }
    UpdateLines(*pref);
    std::cout << "modified\n";
}

void rc::Road::Render()
{
    if (Uf::UnionFind::isRunThread)
    {
        Uf::UnionFind::ProcessBarRender();
        return;
    }
    Game::window.GetWindow()->draw(roadVArray);
    Game::window.GetWindow()->draw(arrows[0]);
    Game::window.GetWindow()->draw(arrows[1]);
}

void rc::Road::RenderAnswer() const
{
    line.Render();
}

void rc::Road::Recenter(Cont::Control& controlers)
{
    pref->Center();
    if (trees.size() != pref->GetMazePointCount() - 1)
    {
        return;
    }
    UpdateLines(*pref);
    controlers.Update(*pref);
}

void rc::Road::Exit()
{
    Uf::UnionFind::Exit();
}
