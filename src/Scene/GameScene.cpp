#include "GameScene.h"


#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <string>

#include "../Util/Variables.h"
void GameScene::InitScene(const std::shared_ptr<Settings>& setting, const std::shared_ptr<Window>& window)
{
    this->window = window;
    Init(window, setting);
    timer.Init(setting, window);
    uf = Uf::UnionFind(setting, window);
    printer = PrintMaze(setting);
    controller = Cont::Control(setting, window);
    settings = setting;
    toggleFileTab = Button::RoundedButton(window, 0, {MENU_BUTTON_WIDTH, MENU_BAR_LIST_CONTENTS_HEIGHT}, 0, "File");
    for (int i = 0; i < 4; i++)
    {
        std::string label;
        switch (i)
        {
        case 0:
            label = "Open";
            break;
        case 1:
            label = "Save";
            break;
        case 2:
            label = "Save as";
            break;
        case 3:
            label = "Quit";
            break;
        }
        fileTabLists[i] = Button::RoundedButton(window, MENU_BAR_ROUND_RES,
                                                {MENU_BAR_LIST_CONTENTS_WIDTH, MENU_BAR_LIST_CONTENTS_HEIGHT},
                                                MENU_BAR_LIST_CONTENTS_ROUND, label);
        fileTabLists[i].SetPosition(sf::Vector2f(MENU_BAR_LIST_OFFSET + MENU_BAR_LIST_CONTENTS_MARGIN,
                                                 MENU_BAR_HEIGHT + MENU_BAR_LIST_OFFSET +
                                                     MENU_BAR_LIST_CONTENTS_MARGIN + MENU_BAR_LIST_CONTENTS_OFFSET * i +
                                                     MENU_BAR_LIST_CONTENTS_HEIGHT * i));
    }
    fileTab = RoundedRectangle(MENU_BAR_ROUND_RES,
                               sf::Vector2f(MENU_BAR_LIST_CONTENTS_MARGIN * 2 + MENU_BAR_LIST_CONTENTS_WIDTH,
                                            MENU_BAR_LIST_CONTENTS_MARGIN * 2 + MENU_BAR_LIST_CONTENTS_OFFSET * 3 +
                                                MENU_BAR_LIST_CONTENTS_HEIGHT * 4),
                               MENU_BAR_LIST_CONTENTS_MARGIN + MENU_BAR_LIST_CONTENTS_ROUND);
    fileTab.setPosition(sf::Vector2f(MENU_BAR_LIST_OFFSET, MENU_BAR_LIST_OFFSET + MENU_BAR_HEIGHT));
    CalcMenuBar();
}

void GameScene::Update()
{
    timer.update();
    if (toggleFileTab.IsActive())
    {
        isFileTabActive = true;
    }
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
            timer.renderTimer();
        }
        window->GetWindow()->draw(menuBarBackground);
        toggleFileTab.Render();
        if (isFileTabActive)
        {
            window->GetWindow()->draw(fileTab);
            for (auto& fileBtns : fileTabLists)
            {
                fileBtns.Render();
            }
        }
    }
}

void GameScene::Event(sf::Event& event)
{
    toggleFileTab.Event(event);

    if (isFileTabActive)
    {
        for (auto& btn : fileTabLists)
        {
            btn.Event(event);
        }

        if (const auto mouse = event.getIf<sf::Event::MouseButtonPressed>())
        {
            if (!fileTab.getGlobalBounds().contains(sf::Vector2f(mouse->position)))
            {
                isFileTabActive = false;
            }
        }
    }
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
                timer.stop();
            }
            else
            {
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
        default:;
        }
    }
    if (event.is<sf::Event::Resized>())
    {
        uf.Recenter(controller);
        CalcMenuBar();
    }
}

void GameScene::CalcMenuBar()
{
    menuBarBackground.setSize(sf::Vector2f(settings->GetWindowSize().x, MENU_BAR_LIST_CONTENTS_HEIGHT));
    menuBarBackground.setFillColor(MENU_BAR_BACKGROUND_COLOR);
}

void GameScene::Exit()
{
    uf.Exit();
}
