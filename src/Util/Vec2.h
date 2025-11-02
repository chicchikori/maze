#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <utility>
struct Vec2Hash
{
    std::size_t operator()(const sf::Vector2i& vector) const
    {
        return (static_cast<size_t>(vector.x) << 32) ^ static_cast<size_t>(vector.y);
    }
};
struct Vec2PairHash
{
    std::size_t operator()(const std::pair<sf::Vector2i, sf::Vector2i>& vector) const
    {
        Vec2Hash hash;
        size_t hash1 = hash(vector.first);
        size_t hash2 = hash(vector.second);
        return hash1 ^ hash2;
    }
};

struct Vec2PairEqual
{
    bool operator()(const std::pair<sf::Vector2i, sf::Vector2i>& a, // NOLINT
                    const std::pair<sf::Vector2i, sf::Vector2i>& b) const noexcept // NOLINT
    {
        return (a.first == b.first && a.second == b.second) || (a.first == b.second && a.second == b.first);
    }
};

inline sf::Vector2f NormalToCenterWindowSize(sf::Vector2f vec2, sf::Vector2f offset, float size)
{
    return sf::Vector2f(sf::Vector2i(offset + sf::Vector2f(size / 2.F, size / 2.F) + vec2 * size));
}

inline sf::Vector2f NormalToWindowSize(sf::Vector2f vec2, sf::Vector2f offset, float size)
{
    return sf::Vector2f(sf::Vector2i(offset + vec2 * size));
}

inline sf::Vector2f GetLocalSize(const sf::Text& text)
{
    const sf::String str = text.getString() + '\n';

    float maxLineWidth = 0.F;
    float lineWidth = 0.F;
    unsigned int lines = 0;

    for (char32_t itr : str)
    {
        if (itr == '\n')
        {
            ++lines;
            maxLineWidth = std::max(maxLineWidth, lineWidth);
            lineWidth = 0.F;
        }
        else
        {
            lineWidth +=
                text.getFont().getGlyph(itr, text.getCharacterSize(), (text.getStyle() & sf::Text::Bold) != 0U).advance;
        }
    }

    const float lineHeight = std::round(text.getFont().getLineSpacing(text.getCharacterSize()));
    return {abs(maxLineWidth), (float)lines * lineHeight};
}
