#include "Timer.h"

#include "../../UI/Fonts.h"

#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <chrono>
#include <format>
#include <string>
#include "../../Util/Variables.h"
#include "../../Game.h"

Timer::Timer() : timerText(Text::Fonts::font, "", MAIN_FONT_SIZE)
{
}
void Timer::Init()
{
    
    timerText.setFillColor(WHITE_COLOR);
}

void Timer::Start()
{
    isUseEnd = false;
    begin = std::chrono::high_resolution_clock::now();
}

void Timer::Stop()
{
    if (!isUseEnd)
    {
        end = std::chrono::high_resolution_clock::now();
        isUseEnd = true;
    }
}

void Timer::Update()
{
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(
        (isUseEnd ? end : std::chrono::high_resolution_clock::now()) - begin);

    std::string timeStr = std::format("{:%H:%M:%S}", time);

    timerText.setString(timeStr);
    timerText.setOrigin({timerText.getLocalBounds().size.x, 0});
    timerText.setPosition(sf::Vector2f((float)Game::window.GetDisplaySize().x, 0));
}

void Timer::RenderTimer() const
{
    Game::window.GetWindow()->draw(timerText);
}
