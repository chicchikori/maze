#pragma once

#include "../Files/Settings.h"

#include <SFML/Graphics.hpp>
#include <memory>

enum class SceneState
{
    Menu,
    MazeGame,
    SaveAsImage,
    Setting,
};

class Scene
{
public:
    Scene() = default;
    Scene(const std::shared_ptr<Window>& window)
    {
        Init(window);
    };
    void Init(const std::shared_ptr<Window>& window);
    virtual ~Scene();
    virtual void Update();
    virtual void Event(sf::Event& event);
    virtual void Render();

private:
    std::shared_ptr<Window> window;
};
