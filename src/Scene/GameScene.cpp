#include "GameScene.h"


#include <SFML/Graphics.hpp>
#include <cassert>
#include "../Files/PrintMaze.h"
#include "Scene.h"



GameScene::GameScene(const std::shared_ptr<Window>& window, const std::shared_ptr<Settings>& setting) :
    Scene(window), settings(setting), window(window), road(window, setting->GetPreferences()),
    textInput(window, 5, {40, 10}, 3)
{
}

void GameScene::InitScene(const std::shared_ptr<Settings>& setting, const std::shared_ptr<Window>& window)
{
    this->window = window;
    settings = setting;

    timer.Init(setting, window);
    road = rc::Road(window, setting->GetPreferences());
    // uf = Uf::UnionFind(setting, window);
    controller = Cont::Control(window);
    menu = Menu::Menu(window);
    textInput = InputText(window, 5, {100, 30}, 3);
    textInput.setPosition({400.F, 500.F});
}

void GameScene::Update()
{
    timer.Update();
    menu.Update();
    road.Update();
    textInput.Update();
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
        textInput.Render();
    }
}

void GameScene::Event(sf::Event& event)
{
    menu.Event(event);
    textInput.Event(event);
    if (auto* const key = event.getIf<sf::Event::KeyPressed>())
    {
        switch (key->code)
        {
        case sf::Keyboard::Key::L:
            settings->Load();
            break;
        case sf::Keyboard::Key::S:
            PrintMaze::Save(settings->GetPreferences());
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
        road.Recenter(controller);
    }
}


void GameScene::Exit()
{
    road.Exit();
}
