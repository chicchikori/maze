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

#define DEFAULT_MAZESIZE (sf::Vector2u(10, 10))


std::filesystem::path Settings::SjisToPath(const std::string& sjis)
{
    const int len = MultiByteToWideChar(CP_ACP, 0, sjis.c_str(), -1, nullptr, 0);
    std::wstring wide(len, 0);
    MultiByteToWideChar(CP_ACP, 0, sjis.c_str(), -1, wide.data(), len);
    return std::filesystem::path{wide};
}

Settings::Settings(const std::shared_ptr<Window>& window) :
    window(window), mazeSize(DEFAULT_MAZESIZE), seeds(), updateSeed(true), size(0)
{
}

void Settings::Load()
{
    std::cout << path << "\n";
    std::string utf8Path;
    if (path.empty())
    {
        utf8Path = "";
    }
    else
    {
        utf8Path = std::string(path.u8string().begin(), path.u8string().end());
    }
    const char* res =
        tinyfd_openFileDialog("select maze file", utf8Path.c_str(), 1, filePattern.cbegin(), "maze file", 0);
    if (res == nullptr)
    {
        return;
    }
    path = SjisToPath(res);
    ImportMaze();
}

void Settings::Save()
{
    std::cout << "save\n";
}

void Settings::SaveAs()
{
    std::cout << "save as\n";
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
        offset =
            sf::Vector2f((static_cast<float>(windowSize.x) - (static_cast<float>(windowMazeSize.x) - 2) * size) / 2.F,
                         size + MENU_BAR_HEIGHT);
    }
    else
    {
        size = windowMazeUSize.x;
        offset = sf::Vector2f(
            size, (static_cast<float>(windowSize.y) - (static_cast<float>(windowMazeSize.y) - 2) * size) / 2.F);
    }
}

void Settings::UpdateSeed()
{
    if (updateSeed)
    {
        std::random_device randD;
        std::mt19937 gen(randD());
        std::uniform_int_distribution dis(0, INT_MAX);
        seeds = dis(gen);
    }
}

void Settings::SetDefault()
{
    seeds = 0;
    updateSeed = true;
    mazeSize = DEFAULT_MAZESIZE;
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
    std::u8string u8str = path.u8string();
    std::string str(u8str.begin(), u8str.end());
    auto title = sf::String(str.c_str());
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
    mazeSize = {settingJson.value("mazeSize", nlohmann::basic_json<>()).value("x", DEFAULT_MAZESIZE.x),
                settingJson.value("mazeSize", nlohmann::basic_json<>()).value("y", DEFAULT_MAZESIZE.y)};
    windowSize = {settingJson.value("windowSize", nlohmann::basic_json<>()).value("width", 500U),
                  settingJson.value("windowSize", nlohmann::basic_json<>()).value("height", 400U)};
    if (settingJson.value("fullscreen", false))
    {
        window->Fullscreen();
    }
    else
    {
        window->Windowed(true, windowSize);
    }
}
