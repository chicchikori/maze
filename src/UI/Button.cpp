#include "Button.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include "../Util/Vec2.h"
#include "Fonts.h"
#include "RoundedRectangle.h"


Button::RoundedButton::RoundedButton(const std::shared_ptr<Window>& window, unsigned int res, sf::Vector2f size,
                                     float round, const std::string& label, sf::Color normalColor, sf::Color hoverColor,
                                     sf::Color activeColor) :
    RoundedRectangle(res, size, round), window(window), normalColor(normalColor), hoverColor(hoverColor),
    activeColor(activeColor), label(Fonts::font, label, 16)
{
    CalcLabelPos();
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
    window->GetWindow()->draw(*this);
    window->GetWindow()->draw(label);
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
    CalcLabelPos();
}

void Button::RoundedButton::SetPosition(sf::Vector2f pos)
{
    RoundedRectangle::setPosition(pos);
    CalcLabelPos();
}

void Button::RoundedButton::CalcLabelPos()
{
    const auto size = GetLocalSize(label);
    label.setOrigin(size / 2.f);
    label.setPosition(this->getGlobalBounds().position + this->getGeometricCenter());
}
