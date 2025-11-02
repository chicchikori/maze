#pragma once

#include "../Files/Settings.h"

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>
#include <string>
class Settings;

class Window
{
public:
    Window() = default;
    void Init(const std::shared_ptr<Settings>& setting);
    [[nodiscard]] std::shared_ptr<sf::RenderWindow> GetWindow() const;
    void Event(sf::Event& event);
    void SetTitle(const sf::String& title);
    void ToggleFullWindow();
    void Windowed(bool setSizePos = false, sf::Vector2u winSize = {0, 0}, sf::Vector2i winPos = {-1, -1});
    void Fullscreen();

private:
    std::shared_ptr<Settings> settings;
    std::shared_ptr<sf::RenderWindow> window;
    std::string title;
    bool isFullSizeWindow;
    sf::Vector2i windowedPos;
    sf::Vector2u windowedSize;
};
