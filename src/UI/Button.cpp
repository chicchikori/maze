#include "Button.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include "../Game.h"
#include "Fonts.h"
#include "RoundedRectangle.h"



Button::RoundedButton::RoundedButton(unsigned int res, sf::Vector2f size, float round, const std::string& label,
                                     Text::LabelAlign align, sf::Color normalColor, sf::Color hoverColor,
                                     sf::Color activeColor) :
    RoundedRectangle(res, size, round), align(align), normalColor(normalColor), hoverColor(hoverColor),
    activeColor(activeColor), label(Text::Fonts::font, label, 16)
{
    this->label.setFillColor(sf::Color::White);
    Text::Fonts::AlignText(align, this->label, *this);
}

void Button::RoundedButton::Event(sf::Event& event)
{
    if (const auto mouse = event.getIf<sf::Event::MouseMoved>())
    {
        isHover = this->getGlobalBounds().contains((sf::Vector2f)mouse->position);
    }
    if (const auto mouse = event.getIf<sf::Event::MouseButtonPressed>())
    {
        isActive = mouse->button == sf::Mouse::Button::Left && isHover;
    }
    if (const auto mouse = event.getIf<sf::Event::MouseButtonReleased>())
    {
        isActive = mouse->button != sf::Mouse::Button::Left;
    }
}

void Button::RoundedButton::Render()
{
    if (isActive)
    {
        this->setFillColor(activeColor);
    }
    else if (isHover)
    {
        this->setFillColor(hoverColor);
    }
    else
    {
        this->setFillColor(normalColor);
    }
    Game::window.GetWindow()->draw(*this);
    Game::window.GetWindow()->draw(label);
}

void Button::RoundedButton::SetColor(sf::Color normalColor, sf::Color hoverColor, sf::Color activeColor)
{
    this->normalColor = normalColor;
    this->hoverColor = hoverColor;
    this->activeColor = activeColor;
}

void Button::RoundedButton::SetSize(sf::Vector2f size)
{
    RoundedRectangle::SetSize(size);
    Text::Fonts::AlignText(align, label, *this);
    RoundedRectangle::update();
}

void Button::RoundedButton::SetPosition(sf::Vector2f pos)
{
    RoundedRectangle::setPosition(pos);
    Text::Fonts::AlignText(align, label, *this);
    RoundedRectangle::update();
}

void Button::RoundedButton::SetLabel(const std::string& label)
{
    this->label.setString(label);
    Text::Fonts::AlignText(align, this->label, *this);
}
