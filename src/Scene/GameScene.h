#pragma once

#include "../Files/PrintMaze.h"
#include "../Maze/ControlMaze/Timer.h"
#include "../Maze/UnionFind/UnionFind.h"
#include "../UI/Button.h"
#include "Scene.h"


#include <SFML/Graphics/RectangleShape.hpp>
#include <array>
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
    void CalcMenuBar();
    std::shared_ptr<Settings> settings;
    std::shared_ptr<Window> window;
    Timer timer;
    Uf::UnionFind uf;

    bool isAnswer = true;

    sf::RectangleShape menuBarBackground;
    Button::RoundedButton toggleFileTab;

    RoundedRectangle fileTab;
    std::array<Button::RoundedButton, 4> fileTabLists;

    bool isFileTabActive{false};

    PrintMaze printer;
    Cont::Control controller;
};
