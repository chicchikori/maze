#include "Dropdown.h"
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <utility>
#include "../Util/Variables.h"


DropDown::DropDown(const std::shared_ptr<Window>& window, const std::vector<std::string>& item, sf::Vector2f position) :
    window(window), position(position)
{
    int index = 0;
    for (const auto& str : item)
    {
        items.try_emplace(
            index,
            Button::RoundedButton(window, DROPDOWN_ROUND_RES, {DROPDOWN_CONTENT_WIDTH, DROPDOWN_CONTENT_HEIGHT},
                                  DROPDOWN_ROUND, str));
        items[index].SetPosition(
            position +
            sf::Vector2f(DROPDOWN_MARGIN,
                         DROPDOWN_MARGIN + ((DROPDOWN_CONTENT_OFFSET + DROPDOWN_CONTENT_HEIGHT) * index)));
        items[index].SetColor(MENU_BAR_BACKGROUND_COLOR, items[index].GetHoverColor(), items[index].GetActiveColor());
        index++;
    }
    background =
        RoundedRectangle(DROPDOWN_ROUND_RES,
                         {DROPDOWN_CONTENT_WIDTH + DROPDOWN_MARGIN * 2,
                          DROPDOWN_MARGIN * 2 + (DROPDOWN_CONTENT_OFFSET + DROPDOWN_CONTENT_HEIGHT) * ((float)index) -
                              DROPDOWN_CONTENT_OFFSET},
                         DROPDOWN_ROUND);
    background.setPosition(position);
    background.setFillColor(MENU_BAR_BACKGROUND_COLOR);
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

void DropDown::Update()
{
    for (auto& btn : items)
    {
        btn.second.IsActive();
        activeId = btn.first;
    }
}

int DropDown::GetActive()
{
    return std::exchange(activeId, -1);
}
