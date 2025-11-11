#pragma once

#include <array>
#include <filesystem>
#include "../Maze/UnionFind/Preference.h"


class Files
{
public:
    static bool SaveMaze(const MazePeference& pref);
    static bool SaveAsMaze(const MazePeference& pref);
    static void LoadMaze(MazePeference& pref);
    static void OpenWithPath(MazePeference& pref, const std::filesystem::path& path);

private:
    static void SaveSettings(const MazePeference& pref);
    static void GetFromJson(const std::string& content, MazePeference& pref);
    static std::string GetUtf8FromPath(const std::filesystem::path& path);
    static std::filesystem::path SjisToPath(const std::string& sjis);


    static std::filesystem::path mazeFilePath;
    static std::array<const char*, 1> filePattern;
};


// namespace Files
