#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include <string>

#include <vector>
#include "Button.h"
#include "Dropdown.h"

#include "../Maze/UnionFind/Preference.h"
namespace Menu
{
    class MenuItem : public DropDown
    {
    public:
        explicit MenuItem(const std::string& label = "", const std::vector<std::string>& item = {}, int ids = -1);

        void Render();
        void Update();
        int ActiveItem() const
        {
            return activeItemNum;
        }
        void Event(sf::Event& event);

    private:
        int id;

        sf::Vector2f size;

        std::string label;
        Button::RoundedButton mainButton;
        int activeItemNum{-1};
        bool isDropDownActive{false};
    };


    class Menu
    {
    public:
        Menu();
        void Render();
        void Update(MazePeference& pref);
        void Event(sf::Event& event);

    private:
        MenuItem menuFile;
        sf::RectangleShape menuBarBackground;
    };
} // namespace Menu
