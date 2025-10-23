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
    Save,
};

class Scene
{
public:
    Scene() = default;
    void Init(const std::shared_ptr<Window>& window, const std::shared_ptr<Settings>& setting);
    virtual ~Scene();
    virtual void Update();
    virtual void Event(sf::Event& event);
    virtual void Render();

protected:
    [[nodiscard]] std::shared_ptr<Settings> GetSetting() const
    {
        return settings;
    };


private:
    std::shared_ptr<Settings> settings;
    std::shared_ptr<Window> window;
};
