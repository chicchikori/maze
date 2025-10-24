#include "PopUp.h"

#include "../Maze/UnionFind/UnionFind.h"
#include "../UI/Fonts.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <format>
#include <memory>

#define popSize sf::Vector2f(400, 100)
#define progressbarWidth 10
#define margin 10.f

PopUp::PopUp() : progressPercentText(Fonts::font, "", 16)
{
}

PopUp::PopUp(const std::shared_ptr<Window>& window, const std::shared_ptr<Settings>& settings) :
    setting(settings), window(window), progressPercentText(Fonts::font, "", 14)
{
    background.setSize(popSize);
    background.setFillColor(sf::Color(10, 10, 10));
    background.setOrigin(popSize / 2.f);

    progressBar.setSize(sf::Vector2f(0, progressbarWidth));
    progressBar.setFillColor(sf::Color::Red);
    progressBar.setOrigin(sf::Vector2f(0, progressbarWidth / 2.f));
    progressBarBackground.setSize(sf::Vector2f(popSize.x - 20.f, progressbarWidth));
    progressBarBackground.setOrigin(sf::Vector2f((popSize.x - 20.f) / 2.f, progressbarWidth / 2.f));
}

void PopUp::PopProgress()
{
    sf::Vector2f center = static_cast<sf::Vector2f>(setting->GetWindowSize()) / 2.f;
    float progressValue = (static_cast<float>(Uf::UnionFind::progress) / static_cast<float>(size));

    progressBar.setSize(sf::Vector2f(progressValue * (popSize.x - margin * 2), progressbarWidth));
    background.setPosition(center);
    progressBarBackground.setPosition(center);
    progressBar.setPosition(center - sf::Vector2f(popSize.x / 2.f - margin, 0));
    progressPercentText.setString(std::format("{}%", static_cast<int>(progressValue * 100.f)));
    progressPercentText.setOrigin(progressPercentText.getLocalBounds().size / 2.f);
    progressPercentText.setPosition(center - sf::Vector2f(0, popSize.y / 2.f));

    window->GetWindow()->draw(background);
    window->GetWindow()->draw(progressBarBackground);
    window->GetWindow()->draw(progressBar);
    window->GetWindow()->draw(progressPercentText);
}
