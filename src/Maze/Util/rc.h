#pragma once
#include <SFML/System/Vector2.hpp>
#include <utility>
namespace rc
{

    using bID = int;

    using Wall = std::pair<bID, bID>;

    class Point
    {
    public:
        explicit Point(const sf::Vector2i position) : position(position)
        {
        }
        [[nodiscard]] sf::Vector2i GetPosition() const
        {
            return position;
        }
        void SetPosition(sf::Vector2i position)
        {
            this->position = position;
        }
        [[nodiscard]] bool IsLeft() const
        {
            return isLeft;
        }

        [[nodiscard]] bool IsTop() const
        {
            return isTop;
        }
        void SetIsLeft()
        {
            isLeft = true;
        }
        void SetIsTop()
        {
            isTop = true;
        }

    private:
        sf::Vector2i position;
        bool isLeft{false};
        bool isTop{false};
    };

    enum class RENDERMODE
    {
        ROAD,
        LINE,
        BOTH
    };
} // namespace rc
