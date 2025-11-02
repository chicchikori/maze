
#include "MenuItem.h"

#include <SFML/Window/Event.hpp>

#include <iostream>
#include <string>

#include "../Game.h"
#include "../Util/Variables.h"
#include "Button.h"
#include "Dropdown.h"

#define MENU_BAR_BUTTON_TOP_MARGIN ((MENU_BAR_HEIGHT - MENU_BAR_BUTTON_HEIGHT) >> 1)
#define MENU_BAR_BUTTON_LEFT_MARGIN ((MENU_BAR_WIDTH - MENU_BAR_BUTTON_WIDTH) >> 1)


namespace Menu
{
    // MenuItem

    MenuItem::MenuItem() : DropDown(nullptr), id(-1)
    {
    }

    MenuItem::MenuItem(const std::shared_ptr<Window>& window, const std::shared_ptr<Settings>& settings,
                       const std::string& label, const std::vector<std::string>& item, int ids) :
        DropDown(
            window, item,
            {(float)ids * MENU_BAR_WIDTH + MENU_BAR_BUTTON_LEFT_MARGIN, MENU_BAR_HEIGHT - MENU_BAR_BUTTON_TOP_MARGIN}),
        id(ids), window(window), setting(settings), label(label)
    {
        mainButton =
            Button::RoundedButton(window, MENU_BAR_BUTTON_ROUND_RES, {MENU_BAR_BUTTON_WIDTH, MENU_BAR_BUTTON_HEIGHT},
                                  MENU_BAR_BUTTON_ROUND, label);
        mainButton.SetPosition({MENU_BAR_WIDTH * (float)ids + MENU_BAR_BUTTON_LEFT_MARGIN, MENU_BAR_BUTTON_TOP_MARGIN});
        mainButton.SetColor(MENU_BAR_BACKGROUND_COLOR, MENU_BAR_HOVER_COLOR, MENU_BAR_ACTIVE_COLOR);
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
        if (isDropDownActive)
        {
            DropDown::Update();
            activeItemNum = DropDown::GetActive();
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
        menuFile = MenuItem(window, settings, "Files", {"Open", "Save", "Save As", "Save Image", "Quit"}, 0);
    }

    void Menu::Render()
    {
        window->GetWindow()->draw(menuBarBackground);
        menuFile.Render();
    }
    void Menu::Update()
    {
        menuFile.Update();
        switch (menuFile.ActiveItem())
        {
        case 0:
            setting->Load();
            break;
        case 1:
            setting->Save();
            break;
        case 2:
            setting->SaveAs();
            break;
        case 3:
            std::cerr << "Save as Image button but is not still configured\n";
            break;
        case 4:
            Game::isOpen = false;
        default:
            break;
        }
    }
    void Menu::Event(sf::Event& event)
    {
        if (event.is<sf::Event::Resized>())
        {
            menuBarBackground.setSize({(float)setting->GetWindowSize().x, MENU_BAR_HEIGHT});
        }
        menuFile.Event(event);
    }
} // namespace Menu
