#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <utility>
#include <vector>
#include "Button.h"
class DropDown
{
public:
    DropDown(const std::shared_ptr<Window>& window, const std::vector<std::string>& item = {},
             sf::Vector2f position = {0, 0});

    void Render();
    void Event(sf::Event& event, bool& isDDActive);
    void Update();
    int GetActive() const;


private:
    // std::unordered_map<int, Button::RoundedButton> items;
    std::unordered_map<int, std::pair<Button::RoundedButton, bool>> items;
    std::shared_ptr<Window> window;
    int activeId{-1};


    RoundedRectangle background;
    sf::Vector2f position;
};
