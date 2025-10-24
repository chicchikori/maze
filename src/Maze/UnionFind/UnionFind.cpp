#include "UnionFind.h"

#include "Road.h"

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <climits>
#include <iostream>
#include <memory>
#include <random>
#include <unordered_set>
#include <utility>

std::atomic<bool> Uf::UnionFind::running(false);
std::atomic<bool> Uf::UnionFind::isRunThread(false);
std::atomic<int> Uf::UnionFind::progress(0);

Uf::Edge::Edge(const std::shared_ptr<Point>& from, const std::shared_ptr<Point>& to, const int weight,
               const bool isHorizon) : from(from), to(to), weight(weight), isHorizen(isHorizon)
{
}

Uf::UnionFind::UnionFind(const std::shared_ptr<Settings>& setting, const std::shared_ptr<Window>& window) :
    settings(setting), window(window), pp(window, setting)
{
}

void Uf::UnionFind::Create(Cont::Control& controller)
{
    settings->UpdateSeed();
    if (!running)
    {
        if (th1.joinable())
        {
            th1.join();
        }
        Init();
        th1 = std::jthread(
            [this, &controller]()
            {
                running = true;
                isRunThread = true;

                pp.SetSize((settings->GetMazeSize().x + 1) * settings->GetMazeSize().y +
                           (settings->GetMazeSize().y + 1) * settings->GetMazeSize().x);
                Find(controller);
                running = false;
                isRunThread = false;
            });
    }
}

void Uf::UnionFind::Init()
{
    road = std::make_shared<rc::Road>(settings, window);
    const auto width = static_cast<int>(settings->GetMazeSize().x);
    const auto height = static_cast<int>(settings->GetMazeSize().y);
    std::seed_seq seq{settings->GetSeeds()};
    std::mt19937 rng;
    rng.seed(seq);
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, INT_MAX);

    // init points
    ID id = 0;
    points.clear();
    points.reserve(width * height);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            auto point = std::make_shared<Point>(id, sf::Vector2i(x, y));
            point->root = point;
            points.emplace_back(point);
            id++;
        }
    }
    edges.clear();
    edges.reserve((width - 1) * height + width * (height - 1));
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width - 1; x++)
        {
            edges.emplace_back(
                std::make_shared<Edge>(points.at(y * width + x), points.at(y * width + x + 1), dist(rng), true));
        }
        if (y == height - 1) continue;
        for (int x = 0; x < width; x++)
        {
            edges.emplace_back(
                std::make_shared<Edge>(points.at(y * width + x), points.at((y + 1) * width + x), dist(rng), false));
        }
    }
}

bool Uf::UnionFind::Find(Cont::Control& controller)
{
    progress = 0;
    std::ranges::sort(edges,
                      [](const std::shared_ptr<Edge>& edge1, const std::shared_ptr<Edge>& edge2)
                      {
                          if (edge1->weight == edge2->weight)
                          {
                              return edge1->from->id < edge2->from->id;
                          }
                          return edge1->weight < edge2->weight;
                      });
    std::cout << edges.size() << "\n";
    for (const auto& edge : edges)
    {
        if (Unite(edge->from, edge->to))
        {
            road->AddEdge(edge);
        }

        if (!isRunThread)
        {
            return false;
        }
        progress += 1;
        // std::cout << progress << "\n";
    }
    road->UpdateLines();
    const auto tree = road->GetTree();
    std::unordered_set<std::pair<sf::Vector2i, sf::Vector2i>, Vec2PairHash, Vec2PairEqual> treeSet;
    for (const auto& treeEdge : tree)
    {
        treeSet.insert(std::pair<sf::Vector2i, sf::Vector2i>(treeEdge->from->posision, treeEdge->to->posision));
    }
    controller = Cont::Control(settings, window, treeSet);
    isGoal = false;
    return true;
}

bool Uf::UnionFind::Unite(const std::shared_ptr<Point>& t1, const std::shared_ptr<Point>& t2)
{
    auto p1 = Root(t1);
    auto p2 = Root(t2);
    if (p1 == p2) return false;
    if (p1->size < p2->size) std::swap(p1, p2);
    p1->size += p2->size;
    p2->root = p1;
    return true;
}

std::shared_ptr<Uf::Point> Uf::UnionFind::Root(const std::shared_ptr<Uf::Point>& point)
{
    if (point->root == point) return point;
    point->root = Root(point->root);
    return point->root;
}

void Uf::UnionFind::ProcessBarRender()
{
    pp.PopProgress();
}

void Uf::UnionFind::Recenter(Cont::Control& controller) const
{
    if (road)
    {
        road->Recenter(controller);
    }
}

void Uf::UnionFind::Exit()
{
    if (th1.joinable())
    {
        th1.join();
    }
}
