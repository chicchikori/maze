#include "GameScene.h"


#include <SFML/Graphics.hpp>

void GameScene::InitScene(const std::shared_ptr<Settings>& setting, const std::shared_ptr<Window>& window)
{
    this->window = window;
    Init(window, setting);
    timer.Init(setting, window);
    uf = Uf::UnionFind(setting, window);
    printer = PrintMaze(setting);
    controller = Cont::Control(setting, window);
    settings = setting;
    dd = Menu::MenuItem(window, setting, "Files", {"Save", "Save As", "Load", "Quit"});
}

void GameScene::Update()
{
    timer.Update();
    dd.Update();
}

void GameScene::Render()
{
    if (Uf::UnionFind::running)
    {
        uf.ProcessBarRender();
    }
    else
    {
        auto const road = uf.GetRoad();
        if (road)
        {
            road->Render();
            if (isAnswer)
            {
                road->RenderAnswer();
            }
            else
            {
                controller.Render();
            }
            timer.RenderTimer();
        }
        dd.Render();
    }
}

void GameScene::Event(sf::Event& event)
{
    dd.Event(event);
    if (auto* const key = event.getIf<sf::Event::KeyPressed>())
    {
        switch (key->code)
        {
        case sf::Keyboard::Key::L:
            settings->Load();
            break;
        case sf::Keyboard::Key::S:
            if (uf.GetRoad())
            {
                printer.AddInfo(uf.GetRoad()->GetBoxes(), uf.GetRoad()->GetAnswer());
                printer.Save();
            }
            break;
        case sf::Keyboard::Key::Space:
            if (!isAnswer)
            {
                timer.Stop();
            }
            else
            {
                uf.Create(controller);
                timer.Start();
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
        default:;
        }
    }
    if (event.is<sf::Event::Resized>())
    {
        uf.Recenter(controller);
    }
}


void GameScene::Exit()
{
    uf.Exit();
}
