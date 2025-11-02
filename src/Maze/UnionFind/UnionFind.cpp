#include "UnionFind.h"

#include "Road.h"

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <climits>
#include <cstddef>
#include <iostream>
#include <memory>
#include <random>
#include <unordered_set>
#include <utility>

std::atomic<bool> Uf::UnionFind::running(false);
std::atomic<bool> Uf::UnionFind::isRunThread(false);
std::atomic<int> Uf::UnionFind::progress(0);


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

                pp.SetSize((int)((settings->GetMazeSize().x + 1) * settings->GetMazeSize().y +
                                 (settings->GetMazeSize().y + 1) * settings->GetMazeSize().x));
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
    ID ids = 0;
    points.clear();
    points.reserve((static_cast<size_t>(width * height)));
    for (int y = 0; y < height; y++) // NOLINT
    {
        for (int x = 0; x < width; x++) // NOLINT
        {
            auto point = std::make_shared<Point>(ids, sf::Vector2i(x, y));
            point->root = point;
            points.emplace_back(point);
            ids++;
        }
    }
    edges.clear();
    edges.reserve((width - 1) * height + width * (height - 1));
    for (int y = 0; y < height; y++) // NOLINT
    {
        for (int x = 0; x < width - 1; x++) // NOLINT
        {
            edges.emplace_back(
                std::make_shared<Edge>(points.at(y * width + x), points.at(y * width + x + 1), dist(rng), true));
        }
        if (y == height - 1)
        {
            continue;
        }
        for (int x = 0; x < width; x++) // NOLINT
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

bool Uf::UnionFind::Unite(const std::shared_ptr<Point>& tree1, const std::shared_ptr<Point>& tree2)
{
    auto point1 = Root(tree1);
    auto point2 = Root(tree2);
    if (point1 == point2)
    {
        return false;
    }
    if (point1->size < point2->size)
    {
        std::swap(point1, point2);
    }
    point1->size += point2->size;
    point2->root = point1;
    return true;
}

std::shared_ptr<Uf::Point> Uf::UnionFind::Root(const std::shared_ptr<Uf::Point>& point) // NOLINT
{
    if (point->root == point)
    {
        return point;
    }
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
