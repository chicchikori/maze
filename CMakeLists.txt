cmake_minimum_required(VERSION 3.10)
project(maze LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

set(FONT_URL "https://github.com/rsms/inter/releases/download/v4.1/Inter-4.1.zip")


FetchContent_Declare(SFML
GIT_REPOSITORY https://github.com/SFML/SFML.git
GIT_TAG master
GIT_SHALLOW ON
EXCLUDE_FROM_ALL
SYSTEM
)
FetchContent_Declare(
    inter
    URL ${FONT_URL}
)
FetchContent_Declare(json 
URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(SFML inter json)



set(SOURCES main.cpp
src/Game.cpp
src/Files/PrintMaze.cpp
src/Files/MazeFiles.cpp
src/Files/tinyfiledialogs.c
src/Maze/ControlMaze/Control.cpp
src/Maze/ControlMaze/Timer.cpp
src/Maze/UnionFind/UnionFind.cpp
src/Maze/UnionFind/Road.cpp
src/Maze/UnionFind/Preference.cpp
src/Maze/UnionFind/Answer.cpp
src/Scene/Scene.cpp
src/Scene/GameScene.cpp
src/Scene/SaveImageScene.cpp
src/UI/Fonts.cpp
src/UI/RoundedRectangle.cpp
src/UI/Button.cpp
src/UI/MenuItem.cpp
src/UI/Dropdown.cpp
src/UI/InputText.cpp
src/Window/PopUp.cpp
src/Window/Window.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})


set(ASSETS_FONT_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Assets/Fonts")

set(FONT_DEST "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Assets/Fonts/Inter.ttf")
set(INTER_TTF ${inter_SOURCE_DIR}/extras/ttf/Inter-Regular.ttf)

add_custom_command(
    OUTPUT ${FONT_DEST}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${ASSETS_FONT_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy ${INTER_TTF} ${FONT_DEST}
    DEPENDS ${INTER_TTF}
    COMMENT "Copying Inter-Regular.ttf to Assets/Fonts/Inter.ttf"
)

add_custom_target(copy_fonts ALL DEPENDS ${FONT_DEST})
add_dependencies(${PROJECT_NAME} copy_fonts)

target_link_libraries(${PROJECT_NAME} PRIVATE  sfml-graphics sfml-window sfml-system nlohmann_json::nlohmann_json)
