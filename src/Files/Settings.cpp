#include "Settings.h"

#include "nlohmann/json_fwd.hpp"
#include "tinyfiledialogs.h"

#include <SFML/System/Vector2.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <windows.h>
#include "../Util/Variables.h"


std::filesystem::path Settings::SjisToPath(const std::string& sjis)
{
    if (sjis.empty())
    {
        return std::filesystem::path{};
    }
    const int len = MultiByteToWideChar(CP_ACP, 0, sjis.c_str(), -1, nullptr, 0);
    std::wstring wide(len, 0);
    MultiByteToWideChar(CP_ACP, 0, sjis.c_str(), -1, wide.data(), len);
    return std::filesystem::path{wide};
}

Settings::Settings(const std::shared_ptr<Window>& window) : window(window)
{
}

Settings::Settings(const std::shared_ptr<Window>& window, const std::string& filePath) : window(window), path(filePath)
{
    ImportMaze();
}

void Settings::Load()
{
    std::cout << path << "\n";
    std::string utf8Path = GetUtf8FromPath(path);
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
    if (path.extension().string() == ".maze")
    {
        if (SaveJsonFile(path))
        {
            return;
        }
    }

    SaveAs();
}

void Settings::SaveAs()
{
    std::string utf8Path = GetUtf8FromPath(path);
    auto res = tinyfd_saveFileDialog("save maze file", utf8Path.c_str(), 1, filePattern.cbegin(), "maze file");
    if (res == nullptr)
    {
        return;
    }
    path = SjisToPath(res);
    if (path.empty())
    {
        return;
    }
    SaveJsonFile(path);
}

std::string Settings::GetUtf8FromPath(const std::filesystem::path& path)
{
    std::string str;
    std::u8string u8str = path.u8string();
    str.reserve(u8str.size());
    for (char8_t chars : u8str)
    {
        str.push_back(static_cast<char>(chars));
    }
    return str;
}


void Settings::ImportMaze()
{
    if (path.extension().string() != ".maze")
    {
        return;
    }
    std::ifstream files(path);
    std::string line;
    std::string res;

    while (std::getline(files, line))
    {
        res += line + "\n";
    }

    GetFromJson(res);
    std::u8string u8str = path.u8string();
    std::string str(u8str.begin(), u8str.end());
    auto title = sf::String(str.c_str());
    title += "  (seed:" + std::to_string(preference.GetSeeds()) + ")";
    window->SetTitle(title);
}

void Settings::GetFromJson(const std::string& content)
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
    preference.SetSeeds(settingJson.value("seed", 0));
    isUpdateSeed = settingJson.value("updateSeed", true);
    preference.SetMazeSize({settingJson.value("mazeSize", nlohmann::basic_json<>()).value("x", DEFAULT_MAZESIZE.x),
                            settingJson.value("mazeSize", nlohmann::basic_json<>()).value("y", DEFAULT_MAZESIZE.y)});

    if (settingJson.value("fullscreen", false))
    {
        window->Fullscreen();
    }
    else
    {
        window->Windowed(true,
                         {settingJson.value("windowSize", nlohmann::basic_json<>()).value("width", 500U),
                          settingJson.value("windowSize", nlohmann::basic_json<>()).value("height", 400U)});
    }
    std::cout << "test\n";
}

bool Settings::SaveJsonFile(std::filesystem::path& savePath)
{
    nlohmann::json saveJson;
    saveJson["seed"] = preference.GetSeeds();
    saveJson["updateSeed"] = isUpdateSeed;
    saveJson["mazeSize"] = {{"x", preference.GetMazeSize().x}, {"y", preference.GetMazeSize().y}};
    saveJson["windowSize"] = {{"width", window->GetWindow()->getSize().x},
                              {"height", window->GetWindow()->getSize().y}};
    saveJson["fullscreen"] = window->IsFullscreen();

    if (!savePath.has_filename())
    {
        return false;
    }
    savePath.replace_extension(".maze");
    std::ofstream file(savePath);
    if (!file.is_open())
    {
        return false;
    }
    file << saveJson.dump(4);
    file.close();
    return true;
}
