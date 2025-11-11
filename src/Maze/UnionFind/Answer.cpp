#include "Answer.h"

#include "../../Game.h"
#include "../../Util/Variables.h"
#include "../../Util/Vec2.h"


#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>
#include <stack>
#include <unordered_set>


std::vector<sf::Vector2f> Answer::SolveMaze(const std::unordered_set<std::shared_ptr<Uf::Tree>>& trees,
                                            sf::Vector2i goal)
{
    using Graph = std::unordered_map<sf::Vector2i, std::vector<sf::Vector2i>, Vec2Hash>;
    auto start = sf::Vector2i(0, 0);

    Graph graph;
    for (auto& tree : trees)
    {
        graph[tree->from->posision].push_back(tree->to->posision);
        graph[tree->to->posision].push_back(tree->from->posision);
    }

    std::unordered_map<sf::Vector2i, sf::Vector2i, Vec2Hash> parent;
    std::unordered_set<sf::Vector2i, Vec2Hash> visited;
    std::stack<sf::Vector2i> stack;

    stack.push(start);
    visited.insert(start);
    parent[start] = {-1, -1};

    bool found = false;
    while (!stack.empty())
    {
        sf::Vector2i cur = stack.top();
        stack.pop();
        if (cur == goal)
        {
            found = true;
            break;
        }

        for (auto const& nxt : graph.at(cur))
        {
            if (!visited.contains(nxt))
            {
                visited.insert(nxt);
                parent[nxt] = cur;
                stack.push(nxt);
            }
        }
    }

    std::vector<sf::Vector2f> path;
    if (!found)
    {
        return path;
    }

    sf::Vector2i cur = goal;
    while (cur.x != -1 && cur.y != -1)
    {
        path.push_back(sf::Vector2f(cur));
        cur = parent[cur];
    }
    return path;
}

void Answer::UpdateLine(const std::unordered_set<std::shared_ptr<Uf::Tree>>& trees, const MazePeference& preferences)
{
    answerLine.clear();
    answerRoute =
        SolveMaze(trees, sf::Vector2i((int)preferences.GetMazeSize().x - 1, (int)preferences.GetMazeSize().y - 1));
    answerRoute.emplace(answerRoute.begin(), (float)preferences.GetMazeSize().x - 0.5F,
                        (float)preferences.GetMazeSize().y - 1);
    answerRoute.emplace_back(-0.5F, 0);
    for (int i = 0; i < answerRoute.size() - 1; i++)
    {
        AddAnswerLine(i, preferences);
    }
}

void Answer::AddAnswerLine(int index, const MazePeference& preferences)
{
    for (int i = 0; i < 2; i++)
    {
        sf::Vertex vert{.position = NormalToCenterWindowSize(answerRoute[index + i], preferences.GetOffset(),
                                                             preferences.GetSize()),
                        .color = RED_COLOR};
        answerLine.append(vert);
    }
}

void Answer::Render() const
{
    Game::window.GetWindow()->draw(answerLine);
}
