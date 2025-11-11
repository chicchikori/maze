#pragma once

#include "../../Files/Settings.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <chrono>
#include <memory>

class Timer
{
public:
    Timer();
    void Init(const std::shared_ptr<Settings>& setting, const std::shared_ptr<Window>& window);
    void Start();
    void Stop();
    void RenderTimer() const;
    void Update();

private:
    std::shared_ptr<Settings> settings;
    std::shared_ptr<Window> window;
    std::chrono::time_point<std::chrono::high_resolution_clock> begin;
    std::chrono::time_point<std::chrono::high_resolution_clock> end;
    bool isUseEnd = false;

    sf::Text timerText;
};
