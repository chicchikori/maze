#include "Dropdown.h"
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>

#define DROPDOWN_ROUND_RES 3
#define DROPDOWN_ROUND 2
#define DROPDOWN_MARGIN 2
#define DROPDOWN_CONTENT_OFFSET 1
#define DROPDOWN_CONTENT_HEIGHT 20
#define DROPDOWN_CONTENT_WIDTH 100

DropDown::DropDown(const std::shared_ptr<Window>& window, const std::vector<std::string>& item, sf::Vector2f position) :
    window(window), position(position)
{
    int i = 0;
    for (const auto& str : item)
    {
        items.try_emplace(
            str,
            Button::RoundedButton(window, DROPDOWN_ROUND_RES, {DROPDOWN_CONTENT_WIDTH, DROPDOWN_CONTENT_HEIGHT},
                                  DROPDOWN_ROUND, str));
        items[str].SetPosition(
            position +
            sf::Vector2f(DROPDOWN_MARGIN, DROPDOWN_MARGIN + (DROPDOWN_CONTENT_OFFSET + DROPDOWN_CONTENT_HEIGHT) * i));
        i++;
    }
    background = RoundedRectangle(
        DROPDOWN_ROUND_RES,
        {DROPDOWN_CONTENT_WIDTH + DROPDOWN_MARGIN * 2,
         DROPDOWN_MARGIN * 2 + DROPDOWN_CONTENT_OFFSET * (i - 1) + DROPDOWN_CONTENT_HEIGHT * ((float)i)},
        DROPDOWN_ROUND);
    background.setPosition(position);
}

void DropDown::Render()
{
    window->GetWindow()->draw(background);
    for (auto& item : items)
    {
        item.second.Render();
    }
}

void DropDown::Event(sf::Event& event, bool& isDDActive)
{
    for (auto& btn : items)
    {
        btn.second.Event(event);
    }

    if (const auto mouse = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (!background.getGlobalBounds().contains(sf::Vector2f(mouse->position)))
        {
            isDDActive = false;
        }
    }
}
