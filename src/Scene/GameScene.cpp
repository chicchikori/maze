#include "GameScene.h"


#include <SFML/Graphics.hpp>
#include <cassert>
#include "Scene.h"


GameScene::GameScene(const std::shared_ptr<Window>& window, const std::shared_ptr<Settings>& setting) :
    Scene(window, setting), settings(setting), window(window), road(setting, window)
{
}

void GameScene::InitScene(const std::shared_ptr<Settings>& setting, const std::shared_ptr<Window>& window)
{
    this->window = window;
    settings = setting;

    timer.Init(setting, window);
    road = rc::Road(setting, window);
    // uf = Uf::UnionFind(setting, window);
    printer = PrintMaze(setting);
    controller = Cont::Control(setting, window);
    menu = Menu::Menu(window, setting);
}

void GameScene::Update()
{
    timer.Update();
    menu.Update();
    road.Update();
    if (!Uf::UnionFind::isRunThread && createNew)
    {
        auto tree = road.GetTree();
        controller.SetTree(tree);

        createNew = false;
    }
}

void GameScene::Render()
{
    road.Render();
    if (!Uf::UnionFind::running)
    {
        if (isAnswer)
        {
            road.RenderAnswer();
        }
        else
        {
            controller.Render();
        }
        timer.RenderTimer();

        menu.Render();
    }
}

void GameScene::Event(sf::Event& event)
{
    menu.Event(event);
    if (auto* const key = event.getIf<sf::Event::KeyPressed>())
    {
        switch (key->code)
        {
        case sf::Keyboard::Key::L:
            settings->Load();
            break;
        case sf::Keyboard::Key::S:
            printer.AddInfo(road.GetBoxes(), road.GetAnswer());
            printer.Save();
            break;
        case sf::Keyboard::Key::Space:
            if (!isAnswer)
            {
                timer.Stop();
            }
            else
            {
                createNew = true;
                road.CreateUf();
                timer.Start();
            }
            isAnswer = !isAnswer;
            break;

            // Control maze

        default:;
        }
        controller.Event(event);
    }
    if (event.is<sf::Event::Resized>())
    {
        // uf.Recenter(controller);
        road.Recenter(controller);
    }
}


void GameScene::Exit()
{
    road.Exit();
}
