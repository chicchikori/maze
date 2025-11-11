#pragma once


#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <chrono>

class Timer
{
public:
    Timer();
    void Init();
    void Start();
    void Stop();
    void RenderTimer() const;
    void Update();

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> begin;
    std::chrono::time_point<std::chrono::high_resolution_clock> end;
    bool isUseEnd = false;

    sf::Text timerText;
};
