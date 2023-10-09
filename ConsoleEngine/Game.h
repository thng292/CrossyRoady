#pragma once
#include <chrono>
#include <iostream>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "Abstract/AbstractGame.h"
#include "Canvas.h"
#include "Navigation.h"

namespace ConsoleGame {
    class Game final : public AbstractGame {
        struct NavigationStackEntry {
            std::unique_ptr<AbstractScreen> Screen;
            bool IsPopup;

            // clang-format off
        NavigationStackEntry(AbstractScreen* ptr, bool isPopup)
            : Screen(ptr), IsPopup(isPopup) {}

            // clang-format on
        };

        std::vector<NavigationStackEntry> naviStack;
        std::unordered_map<std::wstring_view, std::unique_ptr<AbstractScreen>>
            screens;

        HANDLE hStdOut, hGameScreen;
        Navigation navi;
        Canvas canvas;
        Canvas::CanvasBuffer_t backup;  // Preserve a canvas buffer for popup,
                                        // make it more efficient

        const bool shouldSkipFrame = false;

        const uint32_t targetFPS = 60;
        const std::chrono::nanoseconds _targetFrameTime =
            std::chrono::nanoseconds(std::chrono::seconds(1)) / targetFPS;
        const float targetFrameTime = 1.0f / targetFPS;

       public:
        Game() = default;
        void Init() override;
        void Run(const std::wstring_view screenName) override;

        AbstractGame* addScreen(std::unique_ptr<AbstractScreen>) override;

        ~Game();
    };
}  // namespace ConsoleGame
