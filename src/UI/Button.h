#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <string>
#include "../UI/Fonts.h"
#include "RoundedRectangle.h"


namespace Button
{


    class RoundedButton : public RoundedRectangle
    {
    public:
        explicit RoundedButton(unsigned int res = 0, sf::Vector2f size = {}, float round = 0,
                               const std::string& label = "",
                               Text::LabelAlign align = {.rl = Text::AlignRL::CENTER, .tb = Text::AlignUB::CENTER},
                               sf::Color normalColor = {100, 100, 100}, sf::Color hoverColor = {80, 80, 80},
                               sf::Color activeColor = {30, 30, 30});
        void Event(sf::Event& event);
        void Render();
        void SetColor(sf::Color normalColor, sf::Color hoverColor, sf::Color activeColor);
        RoundedRectangle& GetRounded()
        {
            return *this;
        };
        bool IsActive() const
        {
            return isActive;
        };

        sf::Color GetNormalColor() const
        {
            return normalColor;
        }
        sf::Color GetHoverColor() const
        {
            return hoverColor;
        }
        sf::Color GetActiveColor() const
        {
            return activeColor;
        }

        std::string GetLabel() const
        {
            return label.getString();
        }

        void SetSize(sf::Vector2f size) override;
        void SetPosition(sf::Vector2f pos);
        void setPosition(sf::Vector2f position) = delete;

        void SetLabel(const std::string& label);

    private:
        // void CalcLabelPos(LabelAlign align);
        Text::LabelAlign align;
        sf::Color normalColor{100, 100, 100};
        sf::Color hoverColor{80, 80, 80};
        sf::Color activeColor{30, 30, 30};
        sf::Text label;
        bool isHover{false};
        bool isActive{false};
    };
} // namespace Button
