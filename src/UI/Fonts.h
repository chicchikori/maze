#pragma once
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Text.hpp>

namespace Text
{
    enum class AlignRL : int
    {
        LEFT = 0,
        CENTER = 1,
        RIGHT = 2,
    };
    enum class AlignUB : int
    {
        TOP = 0,
        CENTER = 1,
        BOTTOM = 2,
    };
    struct LabelAlign
    {
        AlignRL rl = AlignRL::CENTER;
        AlignUB tb = AlignUB::CENTER;
    };
    class Fonts
    {
    public:
        static sf::Font font;
        static void AlignText(LabelAlign align, sf::Text& label, const sf::Shape& shape);
    };
} // namespace Text
