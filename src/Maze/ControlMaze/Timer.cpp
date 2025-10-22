#include "Timer.h"

#include "../../Util/Fonts.h"

#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <chrono>
#include <format>
#include <string>

Timer::Timer() : timerText(Fonts::font, "", 16) {
}
void Timer::Init(const std::shared_ptr<Settings>& setting,
                 const std::shared_ptr<Window>& window) {
    this->window = window;
    settings = setting;
    timerText.setFillColor(settings->GetWhite());
}

void Timer::start() {
    isUseEnd = false;
    begin = std::chrono::high_resolution_clock::now();
}

void Timer::stop() {
    if (!isUseEnd) {
        end = std::chrono::high_resolution_clock::now();
        isUseEnd = true;
    }
}

void Timer::update() {
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(
        (isUseEnd ? end : std::chrono::high_resolution_clock::now()) - begin);

    std::string timeStr = std::format("{:%H:%M:%S}", time);

    timerText.setString(timeStr);
    timerText.setOrigin({timerText.getLocalBounds().size.x, 0});
    timerText.setPosition(sf::Vector2f(settings->GetWindowSize().x, 0));
}

void Timer::renderTimer() const {
    window->GetWindow()->draw(timerText);
}