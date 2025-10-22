#include "GameScene.h"

#include "Scene.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

void GameScene::InitScene(const std::shared_ptr<Settings>& setting,
                          const std::shared_ptr<Window>& window) {
    this->window = window;
    Init(window, setting);
    timer.Init(setting, window);
    uf = Uf::UnionFind(setting, window);
    printer = PrintMaze(setting);
    controller = Cont::Control(setting, window);
    settings = setting;
}

void GameScene::Update() {
    timer.update();
}

void GameScene::Render() {
    if (Uf::UnionFind::running) {
        pp.PopProgress();
    } else {
        if (uf.GetRoad()) {
            uf.GetRoad()->render();
            if (isAnswer) {
                uf.GetRoad()->renderAnswer();
            } else {
                controller.Render();
            }
            timer.renderTimer();
        }
    }
}

void GameScene::Event(sf::Event& event) {
    if (const auto key = event.getIf<sf::Event::KeyPressed>()) {
        switch (key->code) {
            case sf::Keyboard::Key::L:
                settings->Load();
                break;
            case sf::Keyboard::Key::S:
                if (uf.GetRoad()) {
                    printer.AddInfo(
                        uf.GetRoad()->GetBoxes(), uf.GetRoad()->GetAnswer());
                    printer.Save();
                }
                break;
            case sf::Keyboard::Key::Space:
                if (!isAnswer) {
                    timer.stop();
                } else {
                    uf.Create(controller);
                    timer.start();
                }
                isAnswer = !isAnswer;
                break;

            // Control maze 
            case sf::Keyboard::Key::Left:
                controller.Move(Cont::MoveDir::Left);
                break;
            case sf::Keyboard::Key::Right:
                controller.Move(Cont::MoveDir::Right);
                break;
            case sf::Keyboard::Key::Up:
                controller.Move(Cont::MoveDir::Up);
                break;
            case sf::Keyboard::Key::Down:
                controller.Move(Cont::MoveDir::Down);
                break;
            default: ;
        }
    }
}

void GameScene::Exit() {
    uf.Exit();
}

GameScene::~GameScene() = default;