#include "Game.h"

#include "Files/Settings.h"
#include "Maze/UnionFind/UnionFind.h"
#include "Scene/GameScene.h"
#include "Scene/Scene.h"
#include "Util/Variables.h"
#include "Window/Window.h"

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <atomic>
#include <memory>
#include <minwindef.h>


SceneState Game::scene{SceneState::MazeGame};
bool Game::isOpen{true};
Window Game::window(sf::Vector2u(800, 600));

Game::Game()
{
    settings = std::make_shared<Settings>();
    scene = SceneState::MazeGame;
}

Game::Game(const std::string& filePath)
{
    settings = std::make_shared<Settings>(filePath);
    scene = SceneState::MazeGame;
}

void Game::Init()
{
    game.InitScene(settings->GetPreference());
    saveScene.Init();
    // settings->Load();
}

;

void Game::Run()
{
    Init();
    while (isOpen)
    {
        Event();
        Update();
        Render();
    }
}

void Game::Event()
{
    while (auto event = window.GetWindow()->pollEvent())
    {
        if (event.has_value())
        {
            window.Event(*event);
            if (event->is<sf::Event::Closed>())
            {
                game.Exit();
                Uf::UnionFind::isRunThread = false;
                isOpen = false;
            }

            switch (scene)
            {
                using enum SceneState;
            case Menu:
                break;
            case MazeGame:
                game.Event(*event);
                break;
            case SaveAsImage:
                saveScene.Event(*event);
                break;
            case Setting:
                break;
            }
        }
    }
}

void Game::Update()
{
    switch (scene)
    {
        using enum SceneState;
    case Menu:
        break;
    case MazeGame:
        game.Update();
        break;
    case SaveAsImage:
        saveScene.Update();
        break;
    case Setting:
        break;
    }
}

void Game::Render()
{
    window.GetWindow()->clear(BACKGROUND_COLOR);
    switch (scene)
    {
        using enum SceneState;
    case Menu:
        break;
    case MazeGame:
        game.Render();
        break;
    case SaveAsImage:
        saveScene.Render();
        break;
    case Setting:
        break;
    }
    window.GetWindow()->display();
}
