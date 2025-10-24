#include "RoundedRectangle.h"
#include <SFML/System/Angle.hpp>
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iostream>

RoundedRectangle::RoundedRectangle(unsigned int res, sf::Vector2f size, float round)
{
    SetResolution(res);
    SetSize(size);
    SetRound(round);
}

void RoundedRectangle::SetResolution(unsigned int res)
{
    resolution = res;
    CalcRound();
}

unsigned int RoundedRectangle::GetResolution() const
{
    return resolution;
}

void RoundedRectangle::SetRound(float round)
{
    if (const auto minSize = std::min(mSize.x, mSize.y); round * 2 > minSize)
    {
        mRound = minSize / 2;
    }
    else
    {
        mRound = round;
    }
    CalcRound();
}

float RoundedRectangle::GetRound() const
{
    return mRound;
}

void RoundedRectangle::SetSize(sf::Vector2f size)
{
    mSize = size;
    CalcRound();
}

sf::Vector2f RoundedRectangle::GetSize() const
{
    return mSize;
}

std::size_t RoundedRectangle::getPointCount() const
{
    return mPoints.size();
}

sf::Vector2f RoundedRectangle::getPoint(std::size_t index) const
{
    assert(index < mPoints.size() && "Index is out of bounds");
    return mPoints[index];
}

sf::Vector2f RoundedRectangle::getGeometricCenter() const
{
    return mSize / 2.f;
}

void RoundedRectangle::AddEdge(sf::Vector2f center, sf::Angle startAngle, sf::Angle step)
{
    for (int i = 0; i < resolution + 1; i++)
    {
        mPoints.emplace_back(center.x + std::sin((startAngle + step * -i).asRadians()) * mRound,
                             center.y - std::cos((startAngle + step * -i).asRadians()) * mRound);
    }
}

void RoundedRectangle::CalcRound()
{
    mPoints.clear();
    std::cout << mPoints.size() << "\n";
    sf::Angle step;
    if (mRound != 0)
    {
        step = sf::degrees(90) / resolution;
    }
    else
    {
        step = sf::degrees(90);
    }
    AddEdge({mRound, mRound}, sf::degrees(0), step);
    AddEdge({mRound, mSize.y - mRound}, sf::degrees(270), step);
    AddEdge({mSize.x - mRound, mSize.y - mRound}, sf::degrees(180), step);
    AddEdge({mSize.x - mRound, mRound}, sf::degrees(90), step);
    std::cout << mPoints.size() << "\n";
    update();
}
