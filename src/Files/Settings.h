#pragma once

#include "../Window/Window.h"
#include "nlohmann/json_fwd.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <string>

class Window;

class Settings {
public:
    explicit Settings(const std::shared_ptr<Window>& window);

    void Load();
    void Save();
    void SaveAs();
    void Center();

    [[nodiscard]] sf::Vector2u GetMazeSize() const {
        return mazeSize;
    }

    [[nodiscard]] sf::Vector2u GetWindowSize() const {
        return windowSize;
    }

    [[nodiscard]] int GetSeeds() const {
        return seeds;
    }

    [[nodiscard]] float GetSize() const {
        return size;
    }

    [[nodiscard]] sf::Vector2f GetOffset() const {
        return offset;
    }

    [[nodiscard]] sf::Color GetGreen() const {
        return green;
    }

    [[nodiscard]] sf::Color GetBlack() const {
        return black;
    }

    [[nodiscard]] sf::Color GetRed() const {
        return red;
    }

    [[nodiscard]] sf::Color GetWhite() const {
        return white;
    }

    // Setters (only for mutable fields)
    void SetMazeSize(const sf::Vector2u& sizes) {
        mazeSize = sizes;
    }

    void SetWindowSize(const sf::Vector2u& sizes) {
        windowSize = sizes;
    }

    void SetSeeds(int s) {
        seeds = s;
    }

    void UpdateSeed();

    void SetDefault();

private:
    std::shared_ptr<Window> window;
    sf::Vector2u mazeSize;
    sf::Vector2u windowSize{1000, 800};
    int seeds;
    bool updateSeed;
    float size;
    sf::Vector2f offset;

    const sf::Color green{130, 178, 108};
    const sf::Color black{54, 57, 59};
    const sf::Color red{242, 84, 84};
    const sf::Color white{243, 243, 244};
    void ImportMaze();
    void SetFromJson(const std::string& content);

    std::filesystem::path path;
    const std::filesystem::path settingFile{"./Assets/Setting.json"};
    char const* filePattern[1] = {"*.maze"};
    nlohmann::json settingJson;
};