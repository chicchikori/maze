#include "Game.h"

#include "Files/Settings.h"
#include "Maze/UnionFind/UnionFind.h"
#include "Scene/GameScene.h"
#include "Scene/Scene.h"
#include "Util/Fonts.h"
#include "Window/Window.h"

#include <SFML/Graphics.hpp>
#include <atomic>
#include <memory>

#define INTER_FONT_PATH "./Assets/Fonts/Inter.ttf"

SceneState Game::scene{SceneState::MazeGame};
sf::Font Fonts::font{INTER_FONT_PATH};

Game::Game() {
    isOpen = true;
    window = std::make_shared<Window>();
    settings = std::make_shared<Settings>(window);
}

void Game::Init() {
    game.InitScene(settings, window);
    window->Init(settings);
    settings->Load();
}

;

void Game::Run() {
    Init();
    while (isOpen) {
        Event();
        Update();
        Render();
    }
}

void Game::Event() {
    while (auto event = window->GetWindow()->pollEvent()) {
        if (event.has_value()) {
            window->Event(*event);
            if (event->is<sf::Event::Closed>()) {
                game.Exit();
                Uf::UnionFind::isRunThread = false;
                isOpen = false;
            }

            switch (scene) {
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
                case Load:
                    break;
            }
        }
    }
}

void Game::Update() {
    switch (scene) {
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
        case Load:
            break;
    }
}

void Game::Render() {
    window->GetWindow()->clear(settings->GetBlack());
    switch (scene) {
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
        case Load:
            break;
    }
    window->GetWindow()->display();
}