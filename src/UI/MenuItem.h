#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include <memory>
#include <string>

#include <vector>
#include "../Files/Settings.h"
#include "../Window/Window.h"
#include "Button.h"
#include "Dropdown.h"

namespace Menu
{
    class MenuItem : public DropDown
    {
    public:
        MenuItem();
        MenuItem(const std::shared_ptr<Window>& window, const std::shared_ptr<Settings>& settings,
                 const std::string& label = "", const std::vector<std::string>& item = {}, int ids = -1);

        void Render();
        void Update();
        int ActiveItem() const
        {
            return activeItemNum;
        }
        void Event(sf::Event& event);

    private:
        int id;
        std::shared_ptr<Window> window;
        std::shared_ptr<Settings> setting;

        sf::Vector2f size;

        std::string label;
        Button::RoundedButton mainButton;
        int activeItemNum{-1};
        bool isDropDownActive{false};
    };


    class Menu
    {
    public:
        Menu() = default;
        Menu(const std::shared_ptr<Window>& window, const std::shared_ptr<Settings>& settings);
        void Render();
        void Update();
        void Event(sf::Event& event);

    private:
        std::shared_ptr<Window> window;
        std::shared_ptr<Settings> setting;
        MenuItem menuFile;
        sf::RectangleShape menuBarBackground;
    };
} // namespace Menu
