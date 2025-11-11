#include "Window.h"

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>
#include <windows.h>

Window::Window(const sf::Vector2u& winSize) :
    title("Maze Generator"), isFullSizeWindow(false), windowedPos(100, 100), windowedSize(800, 600)
{
    sf::ContextSettings context;
    context.antiAliasingLevel = 8;
    window = std::make_shared<sf::RenderWindow>(sf::VideoMode(winSize), title, sf::Style::Default, sf::State::Windowed,
                                                context);
}

void Window::Event(sf::Event& event)
{
    if (const auto windowResize = event.getIf<sf::Event::Resized>())
    {
        const sf::FloatRect visibleArea(sf::Vector2f(0, 0), static_cast<sf::Vector2f>(windowResize->size));
        window->setView(sf::View(visibleArea));
    }
    if (const auto key = event.getIf<sf::Event::KeyReleased>())
    {
        if (key->code == sf::Keyboard::Key::F11)
        {
            ToggleFullWindow();
        }
    }
}

void Window::ToggleFullWindow()
{
    if (isFullSizeWindow)
    {
        Windowed();
    }
    else
    {
        Fullscreen();
    }
}

void Window::Windowed(bool setSizePos, sf::Vector2u winSize, sf::Vector2i winPos)
{
    if (!isFullSizeWindow)
    {
        return;
    }
    if (setSizePos)
    {
        windowedSize = winSize;
        if (winPos != sf::Vector2i(-1, -1))
        {
            windowedPos = winPos;
        }
    }
    const auto hwnd = window->getNativeHandle();
    LONG style = GetWindowLong(hwnd, GWL_STYLE);
    style &= ~WS_POPUP;
    style |= WS_OVERLAPPEDWINDOW;
    SetWindowLong(hwnd, GWL_STYLE, style);

    SetWindowPos(hwnd, nullptr, windowedPos.x, windowedPos.y, static_cast<int>(windowedSize.x),
                 static_cast<int>(windowedSize.y), SWP_FRAMECHANGED | SWP_NOZORDER | SWP_NOOWNERZORDER);
    isFullSizeWindow = false;
}

void Window::Fullscreen()
{
    if (isFullSizeWindow)
    {
        return;
    }
    const auto hwnd = window->getNativeHandle();
    windowedPos = window->getPosition();
    windowedSize = window->getSize();

    LONG style = GetWindowLong(hwnd, GWL_STYLE);
    style &= ~(WS_OVERLAPPEDWINDOW);
    style |= WS_POPUP;
    SetWindowLong(hwnd, GWL_STYLE, style);

    const auto hmon = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
    MONITORINFO monitorInfo = {sizeof(monitorInfo)};
    if (GetMonitorInfo(hmon, &monitorInfo))
    {
        SetWindowPos(hwnd, HWND_TOP, monitorInfo.rcMonitor.left, monitorInfo.rcMonitor.top,
                     monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left,
                     monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top, SWP_FRAMECHANGED | SWP_NOOWNERZORDER);
    }
    isFullSizeWindow = true;
}

void Window::SetTitle(const sf::String& titles)
{
    window->setTitle(titles);
    this->title = titles;
}

std::shared_ptr<sf::RenderWindow> Window::GetWindow() const
{
    return window;
}


[[nodiscard]] sf::Vector2u Window::GetDisplaySize() const
{
    return {window->getSize().x, window->getSize().y - MENU_BAR_HEIGHT};
}
