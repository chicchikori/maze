#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "Button.h"
class DropDown
{
public:
    DropDown(const std::shared_ptr<Window>& window, const std::vector<std::string>& item = {},
             sf::Vector2f position = {0, 0});

    void Render();
    void Event(sf::Event& event, bool& isDDActive);

protected:
    std::unordered_map<std::string, Button::RoundedButton> items;
    void CalcItem();

private:
    std::shared_ptr<Window> window;

    RoundedRectangle background;
    sf::Vector2f position;
};
