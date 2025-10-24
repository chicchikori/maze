#pragma once

#include "Window.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <memory>

class PopUp
{
public:
    PopUp();
    explicit PopUp(const std::shared_ptr<Window>& window, const std::shared_ptr<Settings>& settings);
    void PopProgress();

    void SetSize(const int num)
    {
        size = num;
    };

private:
    std::shared_ptr<Settings> setting;
    std::shared_ptr<Window> window;
    sf::Font& GetFont();
    sf::RectangleShape progressBar;
    sf::RectangleShape progressBarBackground;
    sf::RectangleShape background;
    sf::Text progressPercentText;
    int size = 1;
};
