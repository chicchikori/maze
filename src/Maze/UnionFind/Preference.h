#pragma once

#include <SFML/System/Vector2.hpp>
class MazePeference
{
public:
    [[nodiscard]] sf::Vector2u GetMazeSize() const
    {
        return mazeSize;
    }

    [[nodiscard]] int GetMazePointCount() const
    {
        return static_cast<int>(mazeSize.x * mazeSize.y);
    }
    [[nodiscard]] int GetSeeds() const
    {
        return seeds;
    }

    [[nodiscard]] float GetSize() const
    {
        return size;
    }

    [[nodiscard]] sf::Vector2f GetOffset() const
    {
        return offset;
    }
    // Setters (only for mutable fields)
    void SetMazeSize(const sf::Vector2u& sizes)
    {
        mazeSize = sizes;
        goalPos = {(int)mazeSize.x - 1, (int)mazeSize.y - 1};
    }
    void SetSeeds(int seed)
    {
        seeds = seed;
    }

    [[nodiscard]] sf::Vector2i GetGoalPos() const
    {
        return goalPos;
    }

    void Center();
    void UpdateSeed();

    void SetDefault();

private:
    sf::Vector2u mazeSize{10, 10};
    sf::Vector2f offset{0, 0};
    sf::Vector2i goalPos{9, 9};
    float size{10};
    int seeds;
    bool isUpdateSeed{true};
};
