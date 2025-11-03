#include "Dropdown.h"
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <utility>
#include "../Util/Variables.h"
#include "Button.h"


DropDown::DropDown(const std::shared_ptr<Window>& window, const std::vector<std::string>& item, sf::Vector2f position) :
    window(window), position(position)
{
    int index = 0;
    for (const auto& str : item)
    {
        items.try_emplace(index,
                          std::make_pair(Button::RoundedButton(window, DROPDOWN_ROUND_RES,
                                                               {DROPDOWN_CONTENT_WIDTH, DROPDOWN_CONTENT_HEIGHT},
                                                               DROPDOWN_ROUND, str, {.rl = Button::AlignRL::LEFT}),
                                         false));
        items[index].first.SetPosition(
            position +
            sf::Vector2f(DROPDOWN_MARGIN,
                         DROPDOWN_MARGIN + ((DROPDOWN_CONTENT_OFFSET + DROPDOWN_CONTENT_HEIGHT) * index)));
        items[index].first.SetColor(DROPDOWN_NORMAL_COLOR, DROPDOWN_HOVER_COLOR, DROPDOWN_ACTIVE_COLOR);
        index++;
    }
    background =
        RoundedRectangle(DROPDOWN_ROUND_RES,
                         {DROPDOWN_CONTENT_WIDTH + DROPDOWN_MARGIN * 2,
                          DROPDOWN_MARGIN * 2 + (DROPDOWN_CONTENT_OFFSET + DROPDOWN_CONTENT_HEIGHT) * ((float)index) -
                              DROPDOWN_CONTENT_OFFSET},
                         DROPDOWN_ROUND);
    background.setPosition(position);
    background.setFillColor(DROPDOWN_BACKGROUND_COLOR);
}

void DropDown::Render()
{
    window->GetWindow()->draw(background);
    for (auto& item : items)
    {
        item.second.first.Render();
    }
}

void DropDown::Event(sf::Event& event, bool& isDDActive)
{
    for (auto& btn : items)
    {
        btn.second.first.Event(event);
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
    activeId = -1;
    for (auto& btn : items)
    {
        if (btn.second.first.IsActive() && !btn.second.second)
        {
            activeId = btn.first;
        }
        btn.second.second = btn.second.first.IsActive();
    }
}

int DropDown::GetActive() const
{
    return activeId;
}
