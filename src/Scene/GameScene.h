#pragma once

#include "../Maze/ControlMaze/Timer.h"
#include "../Maze/UnionFind/Road.h"
#include "../UI/InputText.h"
#include "../UI/MenuItem.h"
#include "Scene.h"


#include <SFML/Graphics/RectangleShape.hpp>
#include <memory>


class GameScene final : public Scene
{
public:
    GameScene() = default;
    explicit GameScene(const std::shared_ptr<Window>& window, const std::shared_ptr<Settings>& setting);
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
    rc::Road road;
    InputText textInput;

    bool isAnswer{true};
    bool createNew{false};
    Menu::Menu menu;

    Cont::Control controller;
};
