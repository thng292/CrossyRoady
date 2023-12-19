#pragma once
#include <chrono>
#include <iostream>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "Abstract/AbstractScreen.h"
#include "Canvas.h"
#include "Navigation.h"

namespace ConsoleGame {
    class Game {
        using NavigationStackEntry = std::unique_ptr<AbstractScreen>;
        std::vector<NavigationStackEntry> naviStack;
        std::unordered_map<std::wstring_view, std::unique_ptr<AbstractScreen>>
            screens;

        Navigation navi;
        Canvas canvas;

        const uint32_t targetFPS;
        const std::chrono::nanoseconds _targetFrameTime;
        const std::string_view windowName;

       public:
        Game(const std::string_view& winName, uint32_t fps = 60);
        ~Game();
        void Run(const std::wstring_view screenName);

        Game* AddScreen(std::unique_ptr<AbstractScreen> screen);
    };
}  // namespace ConsoleGame
