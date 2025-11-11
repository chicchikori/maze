#include "InputText.h"

#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <string>
#include "RoundedRectangle.h"


InputText::InputText(const std::shared_ptr<Window>& window, unsigned int res, sf::Vector2f size, float round,
                     std::string defaultText) :
    RoundedRectangle(res, size, round), window(window), currentString(std::move(defaultText))
{
}

void InputText::Event(sf::Event& event) // NOLINT
{
    if (const auto mouse = event.getIf<sf::Event::MouseButtonPressed>())
    {
        isActive = this->getGlobalBounds().contains((sf::Vector2f)mouse->position);
    }
    if (isActive)
    {
        if (const auto textEvent = event.getIf<sf::Event::TextEntered>())
        {
            if (textEvent->unicode == 8) // Backspace
            {
                if (cursorPosition > 0 && !currentString.empty())
                {
                    currentString.erase(cursorPosition - 1, 1);
                    cursorPosition--;
                }
            }
            else if (textEvent->unicode >= 32 && textEvent->unicode <= 126) // Printable characters
            {
                std::cout << static_cast<char>(textEvent->unicode) << '\n';
                currentString.insert(cursorPosition, 1, static_cast<char>(textEvent->unicode));
                cursorPosition++;
            }
        }
        if (const auto keyEvent = event.getIf<sf::Event::KeyPressed>())
        {
            if (keyEvent->code == sf::Keyboard::Key::Left)
            {
                if (cursorPosition > 0)
                {
                    cursorPosition--;
                }
            }
            else if (keyEvent->code == sf::Keyboard::Key::Right)
            {
                if (cursorPosition < currentString.size())
                {
                    cursorPosition++;
                }
            }
        }
    }
}

void InputText::Update()
{
    inputText.setString(currentString);
    inputText.setFillColor(BLACK_COLOR);
    inputText.setPosition(this->getPosition() + sf::Vector2f(5.F, 4.F));

    float cursorX = inputText.getPosition().x;
    if (cursorPosition > 0)
    {
        sf::Text tempText = inputText;
        tempText.setString(currentString.substr(0, cursorPosition));
        cursorX += tempText.getLocalBounds().size.x;
    }

    cursor.setSize(sf::Vector2f(1.F, (float)inputText.getCharacterSize()));
    cursor.setFillColor(BLACK_COLOR);
    cursor.setPosition(sf::Vector2f(cursorX, inputText.getPosition().y));
}

void InputText::Render()
{
    window->GetWindow()->draw(*this);
    window->GetWindow()->draw(inputText);
    if (isActive)
    {
        window->GetWindow()->draw(cursor);
    }
}
