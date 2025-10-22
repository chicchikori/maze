#include "PopUp.h"

#include "../Maze/UnionFind/UnionFind.h"
#include "../Util/Fonts.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <format>

#define popSize sf::Vector2f(400, 100)
#define progressbarWidth 10

PopUp::PopUp() : progressPercentText(Fonts::font, "", 16) {
}

PopUp::PopUp(const std::shared_ptr<Window>& window)
    : progressPercentText(Fonts::font, "", 14) {
    background.setSize(popSize);
    this->window = window;
    background.setFillColor(sf::Color(10, 10, 10));
    background.setOrigin(popSize / 2.f);

    progressBar.setSize(sf::Vector2f(0, progressbarWidth));
    progressBar.setFillColor(sf::Color::Red);
    progressBar.setOrigin(sf::Vector2f(0, progressbarWidth / 2.f));
    progressBarBackground.setSize(
        sf::Vector2f(popSize.x - 20.f, progressbarWidth));
    progressBarBackground.setOrigin(
        sf::Vector2f((popSize.x - 20.f) / 2.f, progressbarWidth / 2.f));
}

void PopUp::PopProgress() {
    progressBar.setSize(
        sf::Vector2f((static_cast<float>(Uf::UnionFind::progress) /
                      static_cast<float>(size)) *
                     (popSize.x - 20.f),
                     progressBar.getSize().y));
    background.setPosition(
        static_cast<sf::Vector2f>(window->GetWindow()->getSize()) / 2.f);
    progressBarBackground.setPosition(
        static_cast<sf::Vector2f>(window->GetWindow()->getSize()) / 2.f);
    progressBar.setPosition(
        static_cast<sf::Vector2f>(window->GetWindow()->getSize()) / 2.f -
        sf::Vector2f(popSize.x / 2.f - 10.f, 0));
    progressPercentText.setString(std::format(
        "{}%", static_cast<int>((static_cast<float>(Uf::UnionFind::progress) /
                                 static_cast<float>(size)) *
                                100.f)));
    progressPercentText.setOrigin(progressPercentText.getLocalBounds().size /
                                  2.f);
    progressPercentText.setPosition(
        static_cast<sf::Vector2f>(window->GetWindow()->getSize()) / 2.f -
        sf::Vector2f(0, popSize.y / 2.f));

    window->GetWindow()->draw(background);
    window->GetWindow()->draw(progressBarBackground);
    window->GetWindow()->draw(progressBar);
    window->GetWindow()->draw(progressPercentText);
}