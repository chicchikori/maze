#include "GameScene.h"


#include <SFML/Graphics.hpp>
#include <cassert>
#include "../Files/MazeFiles.h"
#include "../Files/PrintMaze.h"


GameScene::GameScene(const std::shared_ptr<MazePeference>& pref) : pref(pref), road(pref)
{
}

void GameScene::InitScene(const std::shared_ptr<MazePeference>& pref)
{
    this->pref = pref;

    timer.Init();
    pref->Center();
    road = rc::Road(pref);
    controller = Cont::Control();
    menu = Menu::Menu();
}

void GameScene::Update()
{
    timer.Update();
    menu.Update(*pref);
    road.Update();
    if (!Uf::UnionFind::isRunThread && createNew)
    {
        auto tree = road.GetTree();
        controller.SetTree(tree, *pref);

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
            Files::LoadMaze(*pref);
            break;
        case sf::Keyboard::Key::S:
            PrintMaze::Save(pref);
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
        controller.Event(event, *pref);
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
