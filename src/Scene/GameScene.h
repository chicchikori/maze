#pragma once

#include "../Maze/ControlMaze/Timer.h"
#include "../Maze/UnionFind/Road.h"
#include "../UI/MenuItem.h"
#include "Scene.h"


#include <SFML/Graphics/RectangleShape.hpp>
#include <memory>


class GameScene final : public Scene
{
public:
    GameScene() = default;
    explicit GameScene(const std::shared_ptr<MazePeference>& pref);
    void InitScene(const std::shared_ptr<MazePeference>& pref);
    ~GameScene() override = default;
    void Update() override;
    void Render() override;
    void Event(sf::Event& event) override;
    void Exit();

private:
    std::shared_ptr<MazePeference> pref;
    Timer timer;
    rc::Road road;

    bool isAnswer{true};
    bool createNew{false};
    Menu::Menu menu;

    Cont::Control controller;
};
