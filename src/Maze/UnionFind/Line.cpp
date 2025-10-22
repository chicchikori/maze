#include "Line.h"

#include "../../Util/Vec2.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>
#include <stack>
#include <unordered_set>

std::vector<sf::Vector2f> Line::SolveMaze(
    const std::unordered_set<std::shared_ptr<Uf::Edge>>& edges,
    sf::Vector2i start, sf::Vector2i goal) const {
    using Graph =
        std::unordered_map<sf::Vector2i, std::vector<sf::Vector2i>, Vec2Hash>;

    Graph graph;
    for (auto& e : edges) {
        graph[e->from->posision].push_back(e->to->posision);
        graph[e->to->posision].push_back(e->from->posision);
    }

    std::unordered_map<sf::Vector2i, sf::Vector2i, Vec2Hash> parent;
    std::unordered_set<sf::Vector2i, Vec2Hash> visited;
    std::stack<sf::Vector2i> st;

    st.push(start);
    visited.insert(start);
    parent[start] = {-1, -1};

    bool found = false;
    while (!st.empty()) {
        sf::Vector2i cur = st.top();
        st.pop();
        if (cur == goal) {
            found = true;
            break;
        }

        for (auto const& nxt : graph.at(cur)) {
            if (!visited.contains(nxt)) {
                visited.insert(nxt);
                parent[nxt] = cur;
                st.push(nxt);
            }
        }
    }

    std::vector<sf::Vector2f> path;
    if (!found) return path;

    sf::Vector2i cur = goal;
    while (cur.x != -1 && cur.y != -1) {
        path.push_back(sf::Vector2f(cur));
        cur = parent[cur];
    }
    // std::ranges::reverse(path);
    return path;
}

void Line::UpdateLine(
    const std::unordered_set<std::shared_ptr<Uf::Edge>>& trees) {
    linesVArray.clear();
    for (const auto& tree : trees) {
        sf::Vertex v1{settings->GetOffset() +
                      sf::Vector2f(settings->GetSize() / 2.f,
                                   settings->GetSize() / 2.f) +
                      static_cast<sf::Vector2f>(tree->from->posision) *
                      settings->GetSize(),
                      settings->GetWhite()};
        sf::Vertex v2{settings->GetOffset() +
                      sf::Vector2f(settings->GetSize() / 2.f,
                                   settings->GetSize() / 2.f) +
                      static_cast<sf::Vector2f>(tree->to->posision) *
                      settings->GetSize(),
                      settings->GetWhite()};

        linesVArray.append(v1);
        linesVArray.append(v2);
    }
    answerLine.clear();
    answerRoute = SolveMaze(trees, {0, 0},
                            sf::Vector2i(settings->GetMazeSize().x - 1,
                                         settings->GetMazeSize().y - 1));
    answerRoute.emplace(answerRoute.begin(),
                        sf::Vector2f((float)settings->GetMazeSize().x - 0.5f,
                                     (float)settings->GetMazeSize().y - 1));
    answerRoute.emplace_back(sf::Vector2f(-0.5f, 0));
    for (int i = 0; i < answerRoute.size() - 1; i++) {
        sf::Vertex v1{settings->GetOffset() +
                      sf::Vector2f(settings->GetSize() / 2.f,
                                   settings->GetSize() / 2.f) +
                      (sf::Vector2f)answerRoute[i] * settings->GetSize(),
                      settings->GetRed()};
        sf::Vertex v2{
            settings->GetOffset() +
            sf::Vector2f(settings->GetSize() / 2.f,
                         settings->GetSize() / 2.f) +
            (sf::Vector2f)answerRoute[i + 1] * settings->GetSize(),
            settings->GetRed()};
        answerLine.append(v1);
        answerLine.append(v2);
    }
}

void Line::AddAnswerLine(int index) {
    sf::Vertex v1{
        settings->GetOffset() +
        sf::Vector2f(settings->GetSize() / 2.f, settings->GetSize() / 2.f) +
        (sf::Vector2f)answerRoute[index] * settings->GetSize(),
        settings->GetRed()};
    sf::Vertex v2{
        settings->GetOffset() +
        sf::Vector2f(settings->GetSize() / 2.f, settings->GetSize() / 2.f) +
        (sf::Vector2f)answerRoute[index + 1] * settings->GetSize(),
        settings->GetRed()};
    answerLine.append(v1);
    answerLine.append(v2);
}

void Line::Render(bool showAnswer) const {
    if (showAnswer) {
        window->GetWindow()->draw(answerLine);
    } else {
        window->GetWindow()->draw(linesVArray);
    }
}