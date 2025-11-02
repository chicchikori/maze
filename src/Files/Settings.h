#pragma once

#include "../Window/Window.h"
#include "nlohmann/json_fwd.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <string>
#include "../Util/Variables.h"


class Window;

class Settings
{
public:
    explicit Settings(const std::shared_ptr<Window>& window);

    void Load();
    void Save();
    void SaveAs();
    void Center();

    [[nodiscard]] sf::Vector2u GetMazeSize() const
    {
        return mazeSize;
    }

    [[nodiscard]] sf::Vector2u GetWindowSize() const
    {
        return windowSize;
    }

    [[nodiscard]] sf::Vector2u GetDisplaySize() const
    {
        return {windowSize.x, windowSize.y - MENU_BAR_HEIGHT};
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
    }

    void SetWindowSize(const sf::Vector2u& sizes)
    {
        windowSize = sizes;
    }

    void SetSeeds(int seed)
    {
        seeds = seed;
    }

    void UpdateSeed();

    void SetDefault();

    static std::filesystem::path SjisToPath(const std::string& sjis);

private:
    std::shared_ptr<Window> window;
    sf::Vector2u mazeSize;
    sf::Vector2u windowSize{1000, 800};
    int seeds;
    bool updateSeed;
    float size;
    sf::Vector2f offset;

    /* const sf::Color green{130, 178, 108};
    const sf::Color black{54, 57, 59};
    const sf::Color red{242, 84, 84};
    const sf::Color white{243, 243, 244}; */
    void ImportMaze();
    void SetFromJson(const std::string& content);

    std::filesystem::path path;
    std::filesystem::path settingFile{SETTING_FILE_PATH};
    std::array<const char*, 1> filePattern = {"*.maze"};
    nlohmann::json settingJson;
};
