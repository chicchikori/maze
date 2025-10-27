#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstddef>
#include <cstdlib>
#include <utility>
struct Vec2Hash
{
    std::size_t operator()(const sf::Vector2i& v) const
    {
        return (static_cast<size_t>(v.x) << 32) ^ static_cast<size_t>(v.y);
    }
};
struct Vec2PairHash
{
    std::size_t operator()(const std::pair<sf::Vector2i, sf::Vector2i>& v) const
    {
        Vec2Hash h;
        size_t h1 = h(v.first);
        size_t h2 = h(v.second);
        return h1 ^ h2;
    }
};

struct Vec2PairEqual
{
    bool operator()(const std::pair<sf::Vector2i, sf::Vector2i>& a,
                    const std::pair<sf::Vector2i, sf::Vector2i>& b) const noexcept
    {
        return (a.first == b.first && a.second == b.second) || (a.first == b.second && a.second == b.first);
    }
};

inline sf::Vector2f NormalToCenterWindowSize(sf::Vector2f vec2, sf::Vector2f offset, float size)
{
    return sf::Vector2f(sf::Vector2i(offset + sf::Vector2f(size / 2.f, size / 2.f) + vec2 * size));
}

inline sf::Vector2f NormalToWindowSize(sf::Vector2f vec2, sf::Vector2f offset, float size)
{
    return sf::Vector2f(sf::Vector2i(offset + vec2 * size));
}

inline sf::Vector2f GetLocalSize(const sf::Text& text)
{
    const sf::String str = text.getString() + '\n';

    float maxLineWidth = 0.f;
    float lineWidth = 0.f;
    unsigned int lines = 0;

    for (char32_t itr : str)
    {
        if (itr == '\n')
        {
            ++lines;
            maxLineWidth = std::max(maxLineWidth, lineWidth);
            lineWidth = 0.f;
        }
        else
        {
            lineWidth +=
                text.getFont().getGlyph(itr, text.getCharacterSize(), text.getStyle() & sf::Text::Bold).advance;
        }
    }

    const float lineHeight = static_cast<int>(text.getFont().getLineSpacing(text.getCharacterSize()));
    return {abs(maxLineWidth), lines * lineHeight};
}
