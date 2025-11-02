#include "Line.h"

#include "../../Util/Vec2.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>
#include <stack>
#include <unordered_set>

std::vector<sf::Vector2f> Line::SolveMaze(const std::unordered_set<std::shared_ptr<Uf::Edge>>& edges,
                                          sf::Vector2i start, sf::Vector2i goal)
{
    using Graph = std::unordered_map<sf::Vector2i, std::vector<sf::Vector2i>, Vec2Hash>;

    Graph graph;
    for (auto& edge : edges)
    {
        graph[edge->from->posision].push_back(edge->to->posision);
        graph[edge->to->posision].push_back(edge->from->posision);
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
    // std::ranges::reverse(path);
    return path;
}

void Line::UpdateLine(const std::unordered_set<std::shared_ptr<Uf::Edge>>& trees)
{
    linesVArray.clear();
    for (const auto& tree : trees)
    {
        AddMazeLine(sf::Vector2f(tree->from->posision), sf::Vector2f(tree->to->posision));
    }
    answerLine.clear();
    answerRoute =
        SolveMaze(trees, {0, 0}, sf::Vector2i((int)settings->GetMazeSize().x - 1, (int)settings->GetMazeSize().y - 1));
    answerRoute.emplace(answerRoute.begin(), (float)settings->GetMazeSize().x - 0.5F,
                        (float)settings->GetMazeSize().y - 1);
    answerRoute.emplace_back(-0.5F, 0);
    for (int i = 0; i < answerRoute.size() - 1; i++)
    {
        AddAnswerLine(i);
    }
}

void Line::AddAnswerLine(int index)
{
    for (int i = 0; i < 2; i++)
    {
        sf::Vertex vert{
            .position = NormalToCenterWindowSize(answerRoute[index + i], settings->GetOffset(), settings->GetSize()),
            .color = RED_COLOR};
        answerLine.append(vert);
    }
}

void Line::AddMazeLine(sf::Vector2f vecFrom, sf::Vector2f vecTo)
{
    sf::Vertex vertexFrom{.position = NormalToCenterWindowSize(vecFrom, settings->GetOffset(), settings->GetSize()),
                          .color = WHITE_COLOR};
    sf::Vertex vertexTo{.position = NormalToCenterWindowSize(vecTo, settings->GetOffset(), settings->GetSize()),
                        .color = WHITE_COLOR};

    linesVArray.append(vertexFrom);
    linesVArray.append(vertexTo);
}

void Line::Render(bool showAnswer) const
{
    if (showAnswer)
    {
        window->GetWindow()->draw(answerLine);
    }
    else
    {
        window->GetWindow()->draw(linesVArray);
    }
}
