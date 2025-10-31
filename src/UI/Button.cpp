#include "Button.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include "../Util/Vec2.h"
#include "Fonts.h"
#include "RoundedRectangle.h"


Button::RoundedButton::RoundedButton() : align(), label(Fonts::font) {};

Button::RoundedButton::RoundedButton(const std::shared_ptr<Window>& window, unsigned int res, sf::Vector2f size,
                                     float round, const std::string& label, LabelAlign align, sf::Color normalColor,
                                     sf::Color hoverColor, sf::Color activeColor) :
    RoundedRectangle(res, size, round), align(align), window(window), normalColor(normalColor), hoverColor(hoverColor),
    activeColor(activeColor), label(Fonts::font, label, 16)
{
    this->label.setFillColor(sf::Color::White);
    CalcLabelPos(align);
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
    CalcLabelPos(align);
    RoundedRectangle::update();
}

void Button::RoundedButton::SetPosition(sf::Vector2f pos)
{
    RoundedRectangle::setPosition(pos);
    CalcLabelPos(align);
    RoundedRectangle::update();
}

void Button::RoundedButton::CalcLabelPos(LabelAlign align)
{
    const auto size = GetLocalSize(label);
    sf::Vector2i delta;
    switch (align.rl)
    {
    case AlignRL::LEFT:
        delta.x = 0;
        label.setOrigin(sf::Vector2f(0, size.y / 2.f));
        label.setPosition(this->getGlobalBounds().position +
                          sf::Vector2f(sf::Vector2i(0, this->getGeometricCenter().y)));
        break;
    case AlignRL::RIGHT:
        delta.x = 2;
        label.setOrigin(sf::Vector2f(size.x, size.y / 2.f));
        label.setPosition(this->getGlobalBounds().position +
                          sf::Vector2f(sf::Vector2i(this->getGeometricCenter().x * 2, this->getGeometricCenter().y)));
        break;
    case AlignRL::CENTER:
        delta.x = 1;
        break;
    }
    switch (align.tb)
    {
    case Button::AlignUB::BOTTOM:
        delta.y = 2;
        break;
    case Button::AlignUB::TOP:
        delta.y = 0;
        break;
    case Button::AlignUB::CENTER:
        delta.y = 1;
        break;
    }

    label.setOrigin(sf::Vector2f(size.x * delta.x * 0.5f, size.y * delta.y * 0.5f));
    label.setPosition(
        this->getGlobalBounds().position +
        sf::Vector2f(sf::Vector2i(this->getGeometricCenter().x * delta.x, this->getGeometricCenter().y * delta.y)));
}

void Button::RoundedButton::SetLabel(const std::string& label)
{
    this->label.setString(label);
    CalcLabelPos(align);
}
