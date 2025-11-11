#include "Fonts.h"
#include <SFML/Graphics/Font.hpp>

#include "../Util/Variables.h"
#include "../Util/Vec2.h"
sf::Font Text::Fonts::font{ARIAL_FONT_PATH};

void Text::Fonts::AlignText(LabelAlign align, sf::Text& label,const sf::Shape& shape)
{
    const auto size = GetLocalSize(label);
    sf::Vector2i delta((int)align.rl, (int)align.tb);
    label.setOrigin(sf::Vector2f(size.x * (float)delta.x * 0.5F, size.y * (float)delta.y * 0.5F));
    label.setPosition(shape.getGlobalBounds().position +
                      sf::Vector2f(sf::Vector2i((int)shape.getGeometricCenter().x * delta.x,
                                                (int)shape.getGeometricCenter().y * delta.y)));
}
