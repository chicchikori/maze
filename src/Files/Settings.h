#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include "../Files/MazeFiles.h"
#include "../Maze/UnionFind/Preference.h"


class Settings
{
public:
    Settings(const std::string& filePath)
    {
        preference = std::make_shared<MazePeference>();
        Files::OpenWithPath(*preference, std::filesystem::path(filePath));
        // Load settings from file (not implemented here)
    }
    Settings()
    {
        preference = std::make_shared<MazePeference>();
        // Load default settings (not implemented here)
    }

    [[nodiscard]] std::shared_ptr<MazePeference>& GetPreference()
    {
        return preference;
    }


private:
    std::shared_ptr<MazePeference> preference;
};
