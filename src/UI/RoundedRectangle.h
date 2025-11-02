#pragma once

#include <SFML/Graphics/Shape.hpp>
#include <SFML/System/Vector2.hpp>

class RoundedRectangle : public sf::Shape
{
public:
    RoundedRectangle(unsigned int res = 0, sf::Vector2f size = {10, 10}, float round = 0);

    void SetRound(float round);
    float GetRound() const;

    virtual void SetSize(sf::Vector2f size);
    sf::Vector2f GetSize() const;

    void SetResolution(unsigned int res);
    unsigned int GetResolution() const;

    [[nodiscard]] std::size_t getPointCount() const override;
    [[nodiscard]] sf::Vector2f getPoint(std::size_t index) const override;
    [[nodiscard]] sf::Vector2f getGeometricCenter() const override;


private:
    [[nodiscard]] sf::Vector2f AddEdge(sf::Vector2f center, sf::Angle startAngle, sf::Angle step, unsigned int index) const;
    void CalcSRound();
    float mRound{0};
    float sRound{0};
    unsigned int resolution{5};
    sf::Vector2f mSize{0, 0};
};
