#include "Road.h"

#include "Line.h"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <cstddef>
#include <memory>


rc::Road::Road(const std::shared_ptr<Settings>& setting, const std::shared_ptr<Window>& window) :
    settings(setting), window(window), line(setting, window)
{
    Init();
}

void rc::Road::Init()
{
    sf::Vector2i goalPos = sf::Vector2i((int)settings->GetMazeSize().x, (int)settings->GetMazeSize().y - 1);
    settings->Center();
    trees.clear();
    boxes.clear();
    // init box
    boxes.reserve((size_t)(settings->GetMazeSize().x + 1) * (settings->GetMazeSize().y + 1));
    for (int x = 0; x < settings->GetMazeSize().x + 1; x++) // NOLINT
    {
        for (int y = 0; y < settings->GetMazeSize().y + 1; y++) // NOLINT
        {
            auto point = std::make_shared<Point>(sf::Vector2i(x, y));
            if (point->GetPosition().x == settings->GetMazeSize().x)
            {
                point->SetIsTop();
            }
            if (point->GetPosition().y == settings->GetMazeSize().y || point->GetPosition() == sf::Vector2i(0, 0) ||
                point->GetPosition() == goalPos)
            {
                point->SetIsLeft();
            }
            boxes.push_back(point);
        }
    }
}

void rc::Road::AddEdge(const std::shared_ptr<Uf::Edge>& edge)
{
    if (edge->isHorizen)
    {
        boxes[edge->to->posision.x * (settings->GetMazeSize().y + 1) + edge->to->posision.y]->SetIsLeft();
    }
    else
    {
        boxes[edge->to->posision.x * (settings->GetMazeSize().y + 1) + edge->to->posision.y]->SetIsTop();
    }
    trees.insert(edge);
}

void rc::Road::UpdateLines()
{
    line.UpdateLine(trees);
    // wall
    roadVArray.clear();

    for (const auto& box : boxes)
    {
        if (!(box->IsTop()))
        {
            AddRoad({1, 0}, box);
        }
        if (!(box->IsLeft()))
        {
            AddRoad({0, 1}, box);
        }
    }

    AddArrow();
}

void rc::Road::AddRoad(sf::Vector2f pos1, const std::shared_ptr<rc::Point>& box)
{
    sf::Vertex position1{.position = NormalToWindowSize(static_cast<sf::Vector2f>(box->GetPosition()),
                                                        settings->GetOffset(), settings->GetSize()),
                         .color = WHITE_COLOR};
    sf::Vertex position2{.position = NormalToWindowSize(static_cast<sf::Vector2f>(box->GetPosition()) + pos1,
                                                        settings->GetOffset(), settings->GetSize()),
                         .color = WHITE_COLOR};
    roadVArray.append(position1);
    roadVArray.append(position2);
}

void rc::Road::AddArrow()
{
    float clampedSize = std::clamp(settings->GetSize(), 0.F, 60.F);
    sGPos[0] = settings->GetOffset() + sf::Vector2f(0, 0.5F) * settings->GetSize();
    sGPos[1] = settings->GetOffset() +
        ((sf::Vector2f)(settings->GetMazeSize()) + sf::Vector2f(0, -0.5F)) * settings->GetSize();
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

void rc::Road::Render()
{
    using enum rc::RENDERMODE;
    if (mode == LINE || mode == BOTH)
    {
        line.Render(false);
    }
    if (mode == ROAD || mode == BOTH)
    {
        window->GetWindow()->draw(roadVArray);
        window->GetWindow()->draw(arrows[0]);
        window->GetWindow()->draw(arrows[1]);
    }
}

void rc::Road::RenderAnswer() const
{
    line.Render(true);
}

void rc::Road::Recenter(Cont::Control& controlers)
{
    settings->Center();
    UpdateLines();
    controlers.Update();
}

void rc::Road::SetMode(const rc::RENDERMODE modes)
{
    mode = modes;
}
