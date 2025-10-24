#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>
#include <string>
#include "../Window/Window.h"
#include "RoundedRectangle.h"

namespace Button
{
    class RoundedButton : public RoundedRectangle
    {
    public:
        RoundedButton(const std::shared_ptr<Window>& window = nullptr, unsigned int res = 0, sf::Vector2f size = {},
                      float round = 0, const std::string& label = "");
        void Event(sf::Event& event);
        void Render();
        RoundedRectangle& GetRounded()
        {
            return *this;
        };
        bool IsActive() const
        {
            return isActive;
        };

        void SetSize(sf::Vector2f size) override;
        void SetPosition(sf::Vector2f pos);
        void setPosition(sf::Vector2f position) = delete;

    private:
        void CalcLabelPos();
        std::shared_ptr<Window> window;
        sf::Color normalColor{100, 100, 100};
        sf::Color hoverColor{80, 80, 80};
        sf::Color activeColor{30, 30, 30};
        sf::Text label;
        bool isHover{false};
        bool isActive{false};
    };
} // namespace Button
