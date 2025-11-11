#include "UnionFind.h"


#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <climits>
#include <cstddef>
#include <future>
#include <memory>
#include <random>
#include <unordered_set>
#include <utility>
#include <vector>
#include "../Util/uf.h"
#include "Preference.h"

std::atomic<bool> Uf::UnionFind::running(false);
std::atomic<bool> Uf::UnionFind::isRunThread(false);
std::atomic<int> Uf::UnionFind::progress(0);

Uf::UnionFind::UnionFind(const std::shared_ptr<Window>& window) : window(window), pp(window)
{
}

void Uf::UnionFind::Init(const MazePeference& pref)
{
    const auto width = static_cast<int>(pref.GetMazeSize().x);
    const auto height = static_cast<int>(pref.GetMazeSize().y);
    std::seed_seq seq{pref.GetSeeds()};
    std::mt19937 rng;
    rng.seed(seq);
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, INT_MAX);

    // init nodes
    nodes.clear();
    nodes.reserve((static_cast<size_t>(width * height)));
    ID ids = 0;
    for (int y = 0; y < height; y++) // NOLINT
    {
        for (int x = 0; x < width; x++) // NOLINT
        {
            auto point = std::make_shared<Node>(ids++, sf::Vector2i(x, y));
            point->root = point;
            nodes.emplace_back(point);
        }
    }
    trees.clear();
    trees.reserve((width - 1) * height + width * (height - 1));
    for (int y = 0; y < height; y++) // NOLINT
    {
        for (int x = 0; x < width - 1; x++) // NOLINT
        {
            trees.emplace_back(
                std::make_shared<Tree>(nodes.at(y * width + x), nodes.at(y * width + x + 1), dist(rng), true));
        }
        if (y == height - 1)
        {
            continue;
        }
        for (int x = 0; x < width; x++) // NOLINT
        {
            trees.emplace_back(
                std::make_shared<Tree>(nodes.at(y * width + x), nodes.at((y + 1) * width + x), dist(rng), false));
        }
    }
}
void Uf::UnionFind::Create(std::promise<std::unordered_set<std::shared_ptr<Uf::Tree>>>& promiseTree,
                           const MazePeference& pref)
{
    if (!running)
    {
        if (th1.joinable())
        {
            th1.join();
        }
        Init(pref);
        th1 = std::jthread(
            [this, &promiseTree, pref]()
            {
                running = true;
                isRunThread = true;

                pp.SetSize((int)((pref.GetMazeSize().x + 1) * pref.GetMazeSize().y +
                                 (pref.GetMazeSize().y + 1) * pref.GetMazeSize().x));
                promiseTree.set_value(Find());
                running = false;
                isRunThread = false;
            });
    }
}

std::unordered_set<std::shared_ptr<Uf::Tree>> Uf::UnionFind::Create(const MazePeference& pref)
{
    Init(pref);
    isRunThread = true;
    auto res = Find();
    isRunThread = false;
    return res;
}

std::unordered_set<std::shared_ptr<Uf::Tree>> Uf::UnionFind::Find()
{
    progress = 0;
    std::unordered_set<std::shared_ptr<Uf::Tree>> tree;

    std::ranges::sort(trees, [](const std::shared_ptr<Tree>& edge1, const std::shared_ptr<Tree>& edge2)
                      { return edge1->weight < edge2->weight; });
    for (const auto& edge : trees)
    {
        if (Unite(edge->from, edge->to))
        {
            tree.emplace(edge);
        }

        if (!isRunThread)
        {
            return {};
        }
        progress += 1;
    }
    return tree;
}

bool Uf::UnionFind::Unite(const std::shared_ptr<Node>& node1, const std::shared_ptr<Node>& node2)
{
    auto root1 = Root(node1);
    auto root2 = Root(node2);
    if (root1 == root2)
    {
        return false;
    }
    if (root1->size < root2->size)
    {
        std::swap(root1, root2);
    }
    root1->size += root2->size;
    root2->root = root1;
    return true;
}

std::shared_ptr<Uf::Node> Uf::UnionFind::Root(const std::shared_ptr<Uf::Node>& node) // NOLINT
{
    if (node->root == node)
    {
        return node;
    }
    node->root = Root(node->root);
    return node->root;
}

void Uf::UnionFind::ProcessBarRender()
{
    pp.PopProgress();
}


void Uf::UnionFind::Exit()
{
    if (th1.joinable())
    {
        th1.join();
    }
}
