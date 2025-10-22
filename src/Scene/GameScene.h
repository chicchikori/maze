#pragma once

#include "../Files/PrintMaze.h"
#include "../Maze/ControlMaze/Timer.h"
#include "../Maze/UnionFind/UnionFind.h"
#include "Scene.h"

#include <memory>

class GameScene final : public Scene
{
public:
    GameScene() = default;
    void InitScene(const std::shared_ptr<Settings>& setting, const std::shared_ptr<Window>& window);
    ~GameScene() override = default;
    void Update() override;
    void Render() override;
    void Event(sf::Event& event) override;
    void Exit();

private:
    std::shared_ptr<Settings> settings;
    std::shared_ptr<Window> window;
    Timer timer;
    Uf::UnionFind uf;

    bool isAnswer = true;

    PrintMaze printer;
    Cont::Control controller;
};
