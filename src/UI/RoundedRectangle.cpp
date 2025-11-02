#include "RoundedRectangle.h"
#include <SFML/Graphics/Shape.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <cmath>
#include <cstddef>


RoundedRectangle::RoundedRectangle(unsigned int res, sf::Vector2f size, float round)
{
    SetResolution(res);
    SetSize(size);
    SetRound(round);
}

void RoundedRectangle::SetResolution(unsigned int res)
{
    resolution = res;
    update();
}

unsigned int RoundedRectangle::GetResolution() const
{
    return resolution;
}

void RoundedRectangle::SetRound(float round)
{
    mRound = round;
    CalcSRound();
    update();
}

float RoundedRectangle::GetRound() const
{
    return mRound;
}

void RoundedRectangle::SetSize(sf::Vector2f size)
{
    mSize = size;
    CalcSRound();
    update();
}

sf::Vector2f RoundedRectangle::GetSize() const
{
    return mSize;
}

std::size_t RoundedRectangle::getPointCount() const
{
    return (static_cast<std::size_t>(resolution + 1)) * 4;
}

sf::Vector2f RoundedRectangle::getPoint(std::size_t index) const
{
    size_t pointCount = getPointCount();
    assert(index < pointCount && "Index is out of bounds");
    const unsigned int corner = /* std::floor */ (index / /* (pointCount / 4.f) */ (resolution + 1));
    const sf::Angle step = (sRound != 0) ? sf::degrees(90) / (float)resolution : sf::degrees(90);
    const unsigned int localIndex = index % (resolution + 1);

    switch (corner)
    {
    case 0:
        // top left
        return AddEdge({sRound, sRound}, sf::degrees(180), step, localIndex);
        break;
    case 1:
        // top right
        return AddEdge({mSize.x - sRound, sRound}, sf::degrees(270), step, localIndex);
        break;
    case 2:
        // bottom right
        return AddEdge({mSize.x - sRound, mSize.y - sRound}, sf::degrees(0), step, localIndex);
        break;
    case 3:
        // bottom left
        return AddEdge({sRound, mSize.y - sRound}, sf::degrees(90), step, localIndex);
        break;
    default:
        return {0, 0};
    }
}

sf::Vector2f RoundedRectangle::getGeometricCenter() const
{
    return mSize / 2.F;
}

sf::Vector2f RoundedRectangle::AddEdge(sf::Vector2f center, sf::Angle startAngle, sf::Angle step,
                                       unsigned int index) const
{
    return {center.x + std::cos((startAngle + step * (float)index).asRadians()) * sRound,
            center.y + std::sin((startAngle + step * (float)index).asRadians()) * sRound};
}

void RoundedRectangle::CalcSRound()
{
    if (const auto minSize = std::min(mSize.x, mSize.y); mRound * 2 > minSize)
    {
        sRound = minSize / 2;
    }
    else
    {
        sRound = mRound;
    }
}
