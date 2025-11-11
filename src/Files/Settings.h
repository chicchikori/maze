#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <string>
#include "../Maze/UnionFind/Preference.h"
#include "../Util/Variables.h"
#include "../Window/Window.h"
#include "nlohmann/json_fwd.hpp"


class Window;

class Settings
{
public:
    explicit Settings(const std::shared_ptr<Window>& window);
    explicit Settings(const std::shared_ptr<Window>& window, const std::string& filePath);

    void Load();
    void Save();
    void SaveAs();

    MazePeference& GetPreferences()
    {
        return preference;
    };
    


    static std::filesystem::path SjisToPath(const std::string& sjis);

private:
    std::shared_ptr<Window> window;
    MazePeference preference;
    bool isUpdateSeed{true};

    bool SaveJsonFile(std::filesystem::path& savePath);
    void ImportMaze();
    void GetFromJson(const std::string& content);
    static std::string GetUtf8FromPath(const std::filesystem::path& path);

    std::filesystem::path path;
    std::filesystem::path settingFile{SETTING_FILE_PATH};
    std::array<const char*, 1> filePattern = {"*.maze"};
    nlohmann::json settingJson;
};
