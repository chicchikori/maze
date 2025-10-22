#include "Road.h"

#include "Line.h"
#include "UnionFind.h"

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <format>
#include <memory>

std::string tostr(sf::Vector2i vec) {
    return std::format("{}:{}", vec.x, vec.y);
}

rc::Road::Road(const std::shared_ptr<Settings>& setting,
               const std::shared_ptr<Window>& window)
    : settings(setting), line(setting, window) {
    this->window = window;
    Init();
}

void rc::Road::Init() {
    settings->Center();

    trees.clear();
    boxes.clear();
    // init box
    boxes.reserve((settings->GetMazeSize().x + 1) *
                  (settings->GetMazeSize().y + 1));
    for (int x = 0; x < settings->GetMazeSize().x + 1; x++) {
        for (int y = 0; y < settings->GetMazeSize().y + 1; y++) {
            auto p = std::make_shared<Point>(sf::Vector2i(x, y));
            if (p->position.x == settings->GetMazeSize().x) {
                p->isTop = true;
            }
            if (p->position.y == settings->GetMazeSize().y) {
                p->isLeft = true;
            }
            if (p->position == sf::Vector2i(0, 0)) {
                p->isLeft = true;
            } else if (p->position ==
                       sf::Vector2i(settings->GetMazeSize().x,
                                    settings->GetMazeSize().y - 1)) {
                p->isLeft = true;
            }
            boxes.push_back(p);
            ++Uf::UnionFind::progress;
        }
    }
}

void rc::Road::AddEdge(const std::shared_ptr<Uf::Edge>& edge) {
    if (edge->isHorizen) {
        boxes[edge->to->posision.x * (settings->GetMazeSize().y + 1) +
              edge->to->posision.y]
            ->isLeft = true;
    } else {
        boxes[edge->to->posision.x * (settings->GetMazeSize().y + 1) +
              edge->to->posision.y]
            ->isTop = true;
    }
    trees.insert(edge);
}

void rc::Road::UpdateLines() {
    line.UpdateLine(trees);
    // wall
    RoadVArray.clear();

    for (const auto& box : boxes) {
        if (!(box->isTop)) {
            sf::Vertex p1{
                settings->GetOffset() +
                (sf::Vector2f)(box->position) * settings->GetSize(),
                settings->GetWhite()};
            sf::Vertex p2{
                settings->GetOffset() +
                (sf::Vector2f)(box->position + sf::Vector2i(1, 0)) *
                settings->GetSize(),
                settings->GetWhite()};
            RoadVArray.append(p1);
            RoadVArray.append(p2);
        }
        if (!(box->isLeft)) {
            sf::Vertex p1{
                settings->GetOffset() +
                (sf::Vector2f)(box->position) * settings->GetSize(),
                settings->GetWhite()};
            sf::Vertex p2{
                settings->GetOffset() +
                (sf::Vector2f)(box->position + sf::Vector2i(0, 1)) *
                settings->GetSize(),
                settings->GetWhite()};
            RoadVArray.append(p1);
            RoadVArray.append(p2);
        }
    }

    DrawArrow();
}

void rc::Road::DrawArrow() {
    float clampedSize = std::clamp(settings->GetSize(), 0.f, 60.f);
    SGPos[0] =
        settings->GetOffset() + sf::Vector2f(0, 0.5f) * settings->GetSize();
    SGPos[1] =
        settings->GetOffset() +
        ((sf::Vector2f)(settings->GetMazeSize()) + sf::Vector2f(0, -0.5f)) *
        settings->GetSize();
    for (int j = 0; j < 2; j++) {
        sf::Vector2i pos((j - 1) * 100 + 10, 10);
        for (int i = 0; i < 7; i++) {
            pos += arrowPos[i];
            Arrows[j].setPoint(i, (sf::Vector2f)pos / 100.f * clampedSize);
            Arrows[j].setPosition(SGPos[j]);
            Arrows[j].setFillColor(SGColor[j]);
        }
    }
}

void rc::Road::render() {
    using enum rc::RENDERMODE;
    if (mode == LINE || mode == BOTH) {
        line.Render(false);
    }
    if (mode == ROAD || mode == BOTH) {
        window->GetWindow()->draw(RoadVArray);
        window->GetWindow()->draw(Arrows[0]);
        window->GetWindow()->draw(Arrows[1]);
    }
}

void rc::Road::renderAnswer() const {
    line.Render(true);
}

void rc::Road::Recenter(Cont::Control& controlers, bool isGoal) {
    settings->Center();
    UpdateLines();
    controlers.Update();
}

void rc::Road::SetMode(const rc::RENDERMODE modes) {
    mode = modes;
}