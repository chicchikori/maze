#include "MazeFiles.h"
#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <winnls.h>
#include "../Game.h"
#include "../Maze/UnionFind/Preference.h"
#include "../Util/Variables.h"
#include "tinyfiledialogs.h"

std::filesystem::path Files::mazeFilePath;
std::array<const char*, 1> Files::filePattern = {"*.maze"};

std::string Files::GetUtf8FromPath(const std::filesystem::path& path)
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

std::filesystem::path Files::SjisToPath(const std::string& sjis)
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


void Files::LoadMaze(MazePeference& pref)
{
    std::cout << mazeFilePath << "\n";
    std::string utf8Path = GetUtf8FromPath(mazeFilePath);
    const char* res =
        tinyfd_openFileDialog("select maze file", utf8Path.c_str(), 1, filePattern.cbegin(), "maze file", 0);
    if (res == nullptr)
    {
        return;
    }
    mazeFilePath = SjisToPath(res);


    std::ifstream files(mazeFilePath);
    std::string line;
    std::string resT;

    while (std::getline(files, line))
    {
        resT += line + "\n";
    }

    GetFromJson(resT, pref);
}

void Files::GetFromJson(const std::string& content, MazePeference& pref)
{
    nlohmann::json json;
    try
    {
        json = nlohmann::json::parse(content);
    }
    catch (const nlohmann::json::parse_error& e)
    {
        std::cout << e.what() << "\n";
        return;
    }
    pref.SetSeeds(json.value("seed", 0));
    pref.SetUpdateSeed(json.value("updateSeed", true));
    pref.SetMazeSize({json.value("mazeSize", nlohmann::basic_json<>()).value("x", DEFAULT_MAZESIZE.x),
                      json.value("mazeSize", nlohmann::basic_json<>()).value("y", DEFAULT_MAZESIZE.y)});

    if (json.value("fullscreen", false))
    {
        Game::window.Fullscreen();
    }
    else
    {
        Game::window.Windowed(true,
                              {json.value("windowSize", nlohmann::basic_json<>()).value("width", 500U),
                               json.value("windowSize", nlohmann::basic_json<>()).value("height", 400U)});
    }
    CoutPref(pref);
    std::cout << "test\n";
}

bool Files::SaveMaze(const MazePeference& pref)
{
    if (mazeFilePath.empty() || mazeFilePath.extension().string() != ".maze")
    {
        return SaveAsMaze(pref);
    }
    SaveSettings(pref);

    return true;
}

bool Files::SaveAsMaze(const MazePeference& pref)
{
    std::string utf8Path = GetUtf8FromPath(mazeFilePath);
    const char* res =
        tinyfd_saveFileDialog("select save location", utf8Path.c_str(), 1, filePattern.cbegin(), "maze file");
    if (res == nullptr)
    {
        return false;
    }
    mazeFilePath = SjisToPath(res);
    if (mazeFilePath.extension().string() != ".maze")
    {
        mazeFilePath.replace_extension(".maze");
    }
    SaveSettings(pref);
    return true;
}

void Files::SaveSettings(const MazePeference& pref)
{
    CoutPref(pref);
    nlohmann::json saveJson;
    saveJson["seed"] = pref.GetSeeds();
    saveJson["updateSeed"] = pref.GetUpdateSeed();
    saveJson["mazeSize"] = {{"x", pref.GetMazeSize().x}, {"y", pref.GetMazeSize().y}};
    saveJson["fullscreen"] = Game::window.IsFullscreen();
    saveJson["windowSize"] = {{"width", Game::window.GetDisplaySize().x}, {"height", Game::window.GetDisplaySize().y}};

    std::ofstream file = std::ofstream(mazeFilePath);
    file << saveJson.dump(4);
    file.close();
}

void Files::OpenWithPath(MazePeference& pref, const std::filesystem::path& path)
{
    mazeFilePath = path;
    LoadMaze(pref);
}

void Files::CoutPref(const MazePeference& pref)
{
    std::cout << "Seed: " << pref.GetSeeds() << "\n";
    std::cout << "Update Seed: " << (pref.GetUpdateSeed() ? "true" : "false") << "\n";
    std::cout << "Maze Size: " << pref.GetMazeSize().x << ", " << pref.GetMazeSize().y << "\n";
    std::cout << "Offset: " << pref.GetOffset().x << ", " << pref.GetOffset().y << "\n";
}
