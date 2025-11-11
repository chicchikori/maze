#pragma once


#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>
#include <string>
class Settings;

class Window
{
public:
    Window(const sf::Vector2u& winSize);
    void Init(const sf::Vector2u& winSize);
    [[nodiscard]] std::shared_ptr<sf::RenderWindow> GetWindow() const;
    void Event(sf::Event& event);
    void SetTitle(const sf::String& title);
    void ToggleFullWindow();
    void Windowed(bool setSizePos = false, sf::Vector2u winSize = {0, 0}, sf::Vector2i winPos = {-1, -1});
    void Fullscreen();
    [[nodiscard]] bool IsFullscreen() const
    {
        return isFullSizeWindow;
    }
    [[nodiscard]] sf::Vector2u GetDisplaySize() const;

private:
    std::shared_ptr<sf::RenderWindow> window;
    std::string title;
    bool isFullSizeWindow;
    sf::Vector2i windowedPos;
    sf::Vector2u windowedSize;
};
