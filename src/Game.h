#pragma once

#include "Files/Settings.h"
#include "Scene/GameScene.h"
#include "Scene/SaveImageScene.h"
#include "Scene/Scene.h"
#include "Window/Window.h"

#include <memory>
#include <string>

class Game
{
public:
    static bool isOpen;
    Game();
    explicit Game(const std::string& filePath);

    void Run();
    static SceneState scene;
    
    static std::shared_ptr<Window> window;
private:
    GameScene game;
    SaveAsScene saveScene;

    std::shared_ptr<Settings> settings;

    void Event();
    void Update();
    void Render();
    void Init();
};
