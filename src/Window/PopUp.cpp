#include "PopUp.h"

#include "../Maze/UnionFind/UnionFind.h"
#include "../UI/Fonts.h"
#include "../Util/Variables.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include "../Game.h"
#include <format>


PopUp::PopUp() :
     progressPercentText(Text::Fonts::font, "", 14)
{
    background.setSize(popSize);
    background.setFillColor(sf::Color(10, 10, 10));
    background.setOrigin(popSize / 2.F);

    progressBar.setSize(sf::Vector2f(0, progressbarWidth));
    progressBar.setFillColor(sf::Color::Red);
    progressBar.setOrigin(sf::Vector2f(0, progressbarWidth / 2.F));
    progressBarBackground.setSize(sf::Vector2f(popSize.x - 20.F, progressbarWidth));
    progressBarBackground.setOrigin(sf::Vector2f((popSize.x - 20.F) / 2.F, progressbarWidth / 2.F));
}

void PopUp::PopProgress()
{
    sf::Vector2f center = static_cast<sf::Vector2f>(Game::window.GetWindow()->getSize()) / 2.F;
    float progressValue = (static_cast<float>(Uf::UnionFind::progress) / static_cast<float>(size));

    progressBar.setSize(sf::Vector2f(progressValue * (popSize.x - margin * 2), progressbarWidth));
    background.setPosition(center);
    progressBarBackground.setPosition(center);
    progressBar.setPosition(center - sf::Vector2f(popSize.x / 2.F - margin, 0));
    progressPercentText.setString(std::format("{}%", static_cast<int>(progressValue * 100.F)));
    progressPercentText.setOrigin(progressPercentText.getLocalBounds().size / 2.F);
    progressPercentText.setPosition(center - sf::Vector2f(0, popSize.y / 2.F));

    Game::window.GetWindow()->draw(background);
    Game::window.GetWindow()->draw(progressBarBackground);
    Game::window.GetWindow()->draw(progressBar);
    Game::window.GetWindow()->draw(progressPercentText);
}
