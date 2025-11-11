#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <memory>
#include <string>
#include "../UI/Fonts.h"
#include "../Util/Variables.h"
#include "../Window/Window.h"
#include "RoundedRectangle.h"


class InputText : public RoundedRectangle
{
public:
    InputText() = default;
    InputText(const std::shared_ptr<Window>& window, unsigned int res = 0, sf::Vector2f size = {10, 10},
              float round = 0, std::string defaultText = "");
    void Event(sf::Event& event);
    void Update();
    void Render();

private:
    std::shared_ptr<Window> window;
    sf::Text inputText{Text::Fonts::font, "", MAIN_FONT_SIZE};
    bool isActive{false};
    std::string currentString;
    sf::RectangleShape cursor;
    int cursorPosition{0};
    /* std::unordered_map<sf::Keyboard::Scan, std::string> keyMap{
        {sf::Keyboard::Scan::A, "a"},         {sf::Keyboard::Scan::B, "b"},
        {sf::Keyboard::Scan::C, "c"},         {sf::Keyboard::Scan::D, "d"},
        {sf::Keyboard::Scan::E, "e"},         {sf::Keyboard::Scan::F, "f"},
        {sf::Keyboard::Scan::G, "g"},         {sf::Keyboard::Scan::H, "h"},
        {sf::Keyboard::Scan::I, "i"},         {sf::Keyboard::Scan::J, "j"},
        {sf::Keyboard::Scan::K, "k"},         {sf::Keyboard::Scan::L, "l"},
        {sf::Keyboard::Scan::M, "m"},         {sf::Keyboard::Scan::N, "n"},
        {sf::Keyboard::Scan::O, "o"},         {sf::Keyboard::Scan::P, "p"},
        {sf::Keyboard::Scan::Q, "q"},         {sf::Keyboard::Scan::R, "r"},
        {sf::Keyboard::Scan::S, "s"},         {sf::Keyboard::Scan::T, "t"},
        {sf::Keyboard::Scan::U, "u"},         {sf::Keyboard::Scan::V, "v"},
        {sf::Keyboard::Scan::W, "w"},         {sf::Keyboard::Scan::X, "x"},
        {sf::Keyboard::Scan::Y, "y"},         {sf::Keyboard::Scan::Z, "z"},
        {sf::Keyboard::Scan::LBracket, "["},  {sf::Keyboard::Scan::RBracket, "]"},
        {sf::Keyboard::Scan::Semicolon, ";"}, {sf::Keyboard::Scan::Comma, ","},
        {sf::Keyboard::Scan::Period, "."},    {sf::Keyboard::Scan::Apostrophe, "'"},
        {sf::Keyboard::Scan::Slash, "/"},     {sf::Keyboard::Scan::Backslash, "\\"},
        {sf::Keyboard::Scan::Grave, "`"},     {sf::Keyboard::Scan::Equal, "="},
        {sf::Keyboard::Scan::Hyphen, "-"},    {sf::Keyboard::Scan::Space, " "},
        {sf::Keyboard::Scan::Num0, "0"},      {sf::Keyboard::Scan::Num1, "1"},
        {sf::Keyboard::Scan::Num2, "2"},      {sf::Keyboard::Scan::Num3, "3"},
        {sf::Keyboard::Scan::Num4, "4"},      {sf::Keyboard::Scan::Num5, "5"},
        {sf::Keyboard::Scan::Num6, "6"},      {sf::Keyboard::Scan::Num7, "7"},
        {sf::Keyboard::Scan::Num8, "8"},      {sf::Keyboard::Scan::Num9, "9"},
        {sf::Keyboard::Scan::Numpad0, "0"},   {sf::Keyboard::Scan::Numpad1, "1"},
        {sf::Keyboard::Scan::Numpad2, "2"},   {sf::Keyboard::Scan::Numpad3, "3"},
        {sf::Keyboard::Scan::Numpad4, "4"},   {sf::Keyboard::Scan::Numpad5, "5"},
        {sf::Keyboard::Scan::Numpad6, "6"},   {sf::Keyboard::Scan::Numpad7, "7"},
        {sf::Keyboard::Scan::Numpad8, "8"},   {sf::Keyboard::Scan::Numpad9, "9"}};


    std::unordered_map<sf::Keyboard::Scan, std::string> keyMapShift{{sf::Keyboard::Scan::A, "A"},
                                                                    {sf::Keyboard::Scan::B, "B"},
                                                                    {sf::Keyboard::Scan::C, "C"},
                                                                    {sf::Keyboard::Scan::D, "D"},
                                                                    {sf::Keyboard::Scan::E, "E"},
                                                                    {sf::Keyboard::Scan::F, "F"},
                                                                    {sf::Keyboard::Scan::G, "G"},
                                                                    {sf::Keyboard::Scan::H, "H"},
                                                                    {sf::Keyboard::Scan::I, "I"},
                                                                    {sf::Keyboard::Scan::J, "J"},
                                                                    {sf::Keyboard::Scan::K, "K"},
                                                                    {sf::Keyboard::Scan::L, "L"},
                                                                    {sf::Keyboard::Scan::M, "M"},
                                                                    {sf::Keyboard::Scan::N, "N"},
                                                                    {sf::Keyboard::Scan::O, "O"},
                                                                    {sf::Keyboard::Scan::P, "P"},
                                                                    {sf::Keyboard::Scan::Q, "Q"},
                                                                    {sf::Keyboard::Scan::R, "R"},
                                                                    {sf::Keyboard::Scan::S, "S"},
                                                                    {sf::Keyboard::Scan::T, "T"},
                                                                    {sf::Keyboard::Scan::U, "U"},
                                                                    {sf::Keyboard::Scan::V, "V"},
                                                                    {sf::Keyboard::Scan::W, "W"},
                                                                    {sf::Keyboard::Scan::X, "X"},
                                                                    {sf::Keyboard::Scan::Y, "Y"},
                                                                    {sf::Keyboard::Scan::Z, "Z"},
                                                                    {sf::Keyboard::Scan::LBracket, "{"},
                                                                    {sf::Keyboard::Scan::RBracket, "}"},
                                                                    {sf::Keyboard::Scan::Semicolon, "+"},
                                                                    {sf::Keyboard::Scan::Comma, "*"},
                                                                    {sf::Keyboard::Scan::Period, ">"},
                                                                    {sf::Keyboard::Scan::Apostrophe, "'"},
                                                                    {sf::Keyboard::Scan::Slash, "?"},
                                                                    {sf::Keyboard::Scan::Backslash, "\\"},
                                                                    {sf::Keyboard::Scan::Grave, "`"},
                                                                    {sf::Keyboard::Scan::Equal, "="},
                                                                    {sf::Keyboard::Scan::Hyphen, "-"},
                                                                    {sf::Keyboard::Scan::Space, " "},
                                                                    {sf::Keyboard::Scan::Num0, "0"},
                                                                    {sf::Keyboard::Scan::Num1, "!"},
                                                                    {sf::Keyboard::Scan::Num2, "\""},
                                                                    {sf::Keyboard::Scan::Num3, "#"},
                                                                    {sf::Keyboard::Scan::Num4, "$"},
                                                                    {sf::Keyboard::Scan::Num5, "%"},
                                                                    {sf::Keyboard::Scan::Num6, "&"},
                                                                    {sf::Keyboard::Scan::Num7, "'"},
                                                                    {sf::Keyboard::Scan::Num8, "("},
                                                                    {sf::Keyboard::Scan::Num9, ")"},
                                                                    {sf::Keyboard::Scan::Numpad0, "0"},
                                                                    {sf::Keyboard::Scan::Numpad1, "1"},
                                                                    {sf::Keyboard::Scan::Numpad2, "2"},
                                                                    {sf::Keyboard::Scan::Numpad3, "3"},
                                                                    {sf::Keyboard::Scan::Numpad4, "4"},
                                                                    {sf::Keyboard::Scan::Numpad5, "5"},
                                                                    {sf::Keyboard::Scan::Numpad6, "6"},
                                                                    {sf::Keyboard::Scan::Numpad7, "7"},
                                                                    {sf::Keyboard::Scan::Numpad8, "8"},
                                                                    {sf::Keyboard::Scan::Numpad9, "9"},
                                                                    {sf::Keyboard::Scan::NumpadDivide, "/"},
                                                                    {sf::Keyboard::Scan::NumpadMultiply, "*"},
                                                                    {sf::Keyboard::Scan::NumpadMinus, "-"},
                                                                    {sf::Keyboard::Scan::NumpadPlus, "+"},
                                                                    {sf::Keyboard::Scan::NumpadEqual, "="},
                                                                    {sf::Keyboard::Scan::NumpadDecimal, "."}}; */
};
