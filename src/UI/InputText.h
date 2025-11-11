#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <string>
#include "../UI/Fonts.h"
#include "../Util/Variables.h"
#include "RoundedRectangle.h"


class InputText : public RoundedRectangle
{
public:
    InputText() = default;
    explicit InputText(unsigned int res = 0, sf::Vector2f size = {10, 10}, float round = 0,
                       std::string defaultText = "");
    void Event(sf::Event& event);
    void Update();
    void Render();

private:
    sf::Text inputText{Text::Fonts::font, "", MAIN_FONT_SIZE};
    bool isActive{false};
    std::string currentString;
    sf::RectangleShape cursor;
    int cursorPosition{0};
};
