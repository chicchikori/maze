
#include "MenuItem.h"

#include <SFML/Window/Event.hpp>
#include <string>

#include "../Util/Variables.h"
#include "Button.h"
#define MENU_BAR_BUTTON_TOP_MARGIN ((MENU_BAR_HEIGHT - MENU_BAR_BUTTON_HEIGHT) >> 1)
#define MENU_BAR_BUTTON_LEFT_MARGIN ((MENU_BAR_WIDTH - MENU_BAR_BUTTON_WIDTH) >> 1)


namespace Menu
{
    // MenuItem

    MenuItem::MenuItem() : DropDown(nullptr), id(-1)
    {
    }

    MenuItem::MenuItem(const std::shared_ptr<Window>& window, const std::shared_ptr<Settings>& settings,
                       const std::string& label, const std::vector<std::string>& item, int id) :
        DropDown(
            window, item,
            {(float)id * MENU_BAR_WIDTH + MENU_BAR_BUTTON_LEFT_MARGIN, MENU_BAR_HEIGHT - MENU_BAR_BUTTON_TOP_MARGIN}),
        id(id), window(window), setting(settings), label(label)
    {
        mainButton =
            Button::RoundedButton(window, MENU_BAR_BUTTON_ROUND_RES, {MENU_BAR_BUTTON_WIDTH, MENU_BAR_BUTTON_HEIGHT},
                                  MENU_BAR_BUTTON_ROUND, label, {.rl = Button::AlignRL::LEFT});
        mainButton.SetPosition({MENU_BAR_WIDTH * (float)id + MENU_BAR_BUTTON_LEFT_MARGIN, MENU_BAR_BUTTON_TOP_MARGIN});
        mainButton.setFillColor(MENU_BAR_BACKGROUND_COLOR);
    }

    void MenuItem::Event(sf::Event& event)
    {
        mainButton.Event(event);
        if (isDropDownActive)
        {
            DropDown::Event(event, isDropDownActive);
        }
    }

    void MenuItem::Update()
    {
        if (mainButton.IsActive())
        {
            isDropDownActive = true;
        }
    }

    void MenuItem::Render()
    {
        mainButton.Render();

        if (isDropDownActive)
        {
            DropDown::Render();
        }
    }

    // Menu
    Menu::Menu(const std::shared_ptr<Window>& window, const std::shared_ptr<Settings>& settings) :
        window(window), setting(settings)
    {
        menuBarBackground.setSize({(float)setting->GetWindowSize().x, MENU_BAR_HEIGHT});
        menuBarBackground.setFillColor(MENU_BAR_BACKGROUND_COLOR);
        menu[0] = MenuItem(window, settings, "Files", {"Open", "Save", "Save As", "Quit"}, 0);

        menu[1] = MenuItem(window, settings, "Test", {"Opent", "Savet", "Save Ast", "Quitt"}, 1);
    }

    void Menu::Render()
    {
        window->GetWindow()->draw(menuBarBackground);
        for (auto& menuItem : menu)
        {
            menuItem.Render();
        }
    }
    void Menu::Update()
    {
        for (auto& menuItem : menu)
        {
            menuItem.Update();
        }
    }
    void Menu::Event(sf::Event& event)
    {
        if (event.is<sf::Event::Resized>())
        {
            menuBarBackground.setSize({(float)setting->GetWindowSize().x, MENU_BAR_HEIGHT});
        }
        for (auto& menuItem : menu)
        {
            menuItem.Event(event);
        }
    }
} // namespace Menu
