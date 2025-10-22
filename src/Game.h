#pragma once

#include "Files/Settings.h"
#include "Scene/GameScene.h"
#include "Scene/Scene.h"
#include "Window/Window.h"

#include <memory>

class Game {
public:
    Game();

    void Run();
    static SceneState scene;

private:
    GameScene game;
    std::shared_ptr<Window> window;

    std::shared_ptr<Settings> settings;

    void Event();
    void Update();
    void Render();
    void Init();

    bool isOpen;
};