#include "Game.h"

#include "Files/Settings.h"
#include "Maze/UnionFind/UnionFind.h"
#include "Scene/GameScene.h"
#include "Scene/Scene.h"
#include "Util/Variables.h"
#include "Window/Window.h"

#include <SFML/Graphics.hpp>
#include <atomic>
#include <memory>


SceneState Game::scene{SceneState::MazeGame};

Game::Game() : isOpen(true)
{
    window = std::make_shared<Window>();
    settings = std::make_shared<Settings>(window);
}

void Game::Init()
{
    game.InitScene(settings, window);
    window->Init(settings);
    settings->Load();
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
    while (auto event = window->GetWindow()->pollEvent())
    {
        if (event.has_value())
        {
            window->Event(*event);
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
                break;
            case Setting:
                break;
            case Save:
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
        break;
    case Setting:
        break;
    case Save:
        break;
    }
}

void Game::Render()
{
    window->GetWindow()->clear(BACKGROUND_COLOR);
    switch (scene)
    {
        using enum SceneState;
    case Menu:
        break;
    case MazeGame:
        game.Render();
        break;
    case SaveAsImage:
        break;
    case Setting:
        break;
    case Save:
        break;
    }
    window->GetWindow()->display();
}
