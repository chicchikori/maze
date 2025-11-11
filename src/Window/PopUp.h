#pragma once


#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

class PopUp
{
public:
    PopUp();
    void PopProgress();

    void SetSize(const int num)
    {
        size = num;
    };

private:
    sf::Font& GetFont();
    sf::RectangleShape progressBar;
    sf::RectangleShape progressBarBackground;
    sf::RectangleShape background;
    sf::Text progressPercentText;
    int size = 1;
};
