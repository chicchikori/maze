#include "Settings.h"

#include "nlohmann/json_fwd.hpp"
#include "tinyfiledialogs.h"

#include <SFML/System/Vector2.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <random>
#include <string>
#include <windows.h>


std::filesystem::path Settings::SjisToPath(const std::string& sjis)
{
    const int len = MultiByteToWideChar(CP_ACP, 0, sjis.c_str(), -1, nullptr, 0);
    std::wstring wide(len, 0);
    MultiByteToWideChar(CP_ACP, 0, sjis.c_str(), -1, wide.data(), len);
    return std::filesystem::path{wide};
}

Settings::Settings(const std::shared_ptr<Window>& window) :
    window(window), mazeSize(100, 100), seeds(), updateSeed(false), size(0)
{
}

void Settings::Load()
{
    const char* res = tinyfd_openFileDialog("select maze file", reinterpret_cast<const char*>(path.c_str()), 1,
                                            filePattern.cbegin(), "maze file", 0);
    if (!res)
    {
        return;
    }
    path = SjisToPath(res);
    ImportMaze();
}

void Settings::Save()
{
}

void Settings::SaveAs()
{
}

void Settings::Center()
{
    const auto windowMazeSize = mazeSize + sf::Vector2u(2, 2);
    const sf::Vector2f windowMazeUSize(
        static_cast<float>(windowSize.x) / static_cast<float>(windowMazeSize.x),
        static_cast<float>(windowSize.y - MENU_BAR_HEIGHT) / static_cast<float>(windowMazeSize.y));

    if (windowMazeUSize.x > windowMazeUSize.y)
    {
        size = windowMazeUSize.y;
        offset = sf::Vector2f(
            (static_cast<float>(windowSize.x) - (static_cast<float>(windowMazeSize.x) - 2) * size) / 2.f, size+MENU_BAR_HEIGHT);
    }
    else
    {
        size = windowMazeUSize.x;
        offset = sf::Vector2f(
            size,
            (static_cast<float>(windowSize.y) - (static_cast<float>(windowMazeSize.y) - 2) * size) /
                2.f);
    }
}

void Settings::UpdateSeed()
{
    if (updateSeed)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution dis(0, INT_MAX);
        seeds = dis(gen);
    }
}

void Settings::SetDefault()
{
    seeds = 0;
    updateSeed = true;
    mazeSize = {10, 10};
    windowSize = {500, 400};
    window->Windowed(true, windowSize);
}

void Settings::ImportMaze()
{
    std::ifstream files(path);
    std::string line;
    std::string res;

    while (std::getline(files, line))
    {
        res += line + "\n";
    }

    SetFromJson(res);
    auto title = sf::String(reinterpret_cast<const char*>(path.u8string().c_str()));
    title += "  (seed:" + std::to_string(seeds) + ")";
    window->SetTitle(title);
}

void Settings::SetFromJson(const std::string& content)
{
    try
    {
        settingJson = nlohmann::json::parse(content);
    }
    catch (const nlohmann::json::parse_error& e)
    {
        std::cout << e.what() << "\n";
        return;
    }
    seeds = settingJson.value("seed", 0);
    updateSeed = settingJson.value("updateSeed", true);
    mazeSize = {settingJson.value("mazeSize", nlohmann::basic_json<>()).value("x", 100u),
                settingJson.value("mazeSize", nlohmann::basic_json<>()).value("y", 100u)};
    windowSize = {settingJson.value("windowSize", nlohmann::basic_json<>()).value("width", 500u),
                  settingJson.value("windowSize", nlohmann::basic_json<>()).value("height", 400u)};
    if (settingJson.value("fullscreen", false))
    {
        window->Fullscreen();
    }
    else
    {
        window->Windowed(true, windowSize);
    }
}
