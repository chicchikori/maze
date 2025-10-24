#include "Road.h"

#include "Line.h"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <memory>


rc::Road::Road(const std::shared_ptr<Settings>& setting, const std::shared_ptr<Window>& window) :
    settings(setting), line(setting, window)
{
    this->window = window;
    Init();
}

void rc::Road::Init()
{
    sf::Vector2i goalPos = sf::Vector2i(settings->GetMazeSize().x, settings->GetMazeSize().y - 1);
    settings->Center();
    trees.clear();
    boxes.clear();
    // init box
    boxes.reserve((settings->GetMazeSize().x + 1) * (settings->GetMazeSize().y + 1));
    for (int x = 0; x < settings->GetMazeSize().x + 1; x++)
    {
        for (int y = 0; y < settings->GetMazeSize().y + 1; y++)
        {
            auto p = std::make_shared<Point>(sf::Vector2i(x, y));
            if (p->position.x == settings->GetMazeSize().x)
            {
                p->isTop = true;
            }
            if (p->position.y == settings->GetMazeSize().y)
            {
                p->isLeft = true;
            }
            if (p->position == sf::Vector2i(0, 0))
            {
                p->isLeft = true;
            }
            else if (p->position == goalPos)
            {
                p->isLeft = true;
            }
            boxes.push_back(p);
        }
    }
}

void rc::Road::AddEdge(const std::shared_ptr<Uf::Edge>& edge)
{
    if (edge->isHorizen)
    {
        boxes[edge->to->posision.x * (settings->GetMazeSize().y + 1) + edge->to->posision.y]->isLeft = true;
    }
    else
    {
        boxes[edge->to->posision.x * (settings->GetMazeSize().y + 1) + edge->to->posision.y]->isTop = true;
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
        if (!(box->isTop))
        {
            AddRoad({1, 0}, box);
        }
        if (!(box->isLeft))
        {
            AddRoad({0, 1}, box);
        }
    }

    AddArrow();
}

void rc::Road::AddRoad(sf::Vector2f pos1, const std::shared_ptr<rc::Point>& box)
{
    sf::Vertex position1{.position = NormalToWindowSize(static_cast<sf::Vector2f>(box->position), settings->GetOffset(),
                                                        settings->GetSize()),
                         .color = settings->GetWhite()};
    sf::Vertex position2{.position = NormalToWindowSize(static_cast<sf::Vector2f>(box->position) + pos1,
                                                        settings->GetOffset(), settings->GetSize()),
                         .color = settings->GetWhite()};
    roadVArray.append(position1);
    roadVArray.append(position2);
}

void rc::Road::AddArrow()
{
    float clampedSize = std::clamp(settings->GetSize(), 0.f, 60.f);
    sGPos[0] = settings->GetOffset() + sf::Vector2f(0, 0.5f) * settings->GetSize();
    sGPos[1] = settings->GetOffset() +
        ((sf::Vector2f)(settings->GetMazeSize()) + sf::Vector2f(0, -0.5f)) * settings->GetSize();
    for (int j = 0; j < 2; j++)
    {
        sf::Vector2i pos((j - 1) * 100 + 10, 10);
        for (int i = 0; i < 7; i++)
        {
            pos += arrowPos[i];
            arrows[j].setPoint(i, (sf::Vector2f)pos / 100.f * clampedSize);
            arrows[j].setPosition(sGPos[j]);
            arrows[j].setFillColor(sGColor[j]);
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
