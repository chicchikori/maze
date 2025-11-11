#pragma once


#include <SFML/Graphics.hpp>

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
    Scene()
    {
        Init();
    };
    void Init();
    virtual ~Scene();
    virtual void Update();
    virtual void Event(sf::Event& event);
    virtual void Render();

private:
};
