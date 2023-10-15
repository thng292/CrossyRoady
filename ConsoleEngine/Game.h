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

            NavigationStackEntry(AbstractScreen* ptr, bool isPopup)
                : Screen(ptr), IsPopup(isPopup)
            {
            }
        };

        std::vector<NavigationStackEntry> naviStack;
        std::unordered_map<std::wstring_view, std::unique_ptr<AbstractScreen>>
            screens;

        HANDLE hStdOut, hGameScreen;
        CONSOLE_SCREEN_BUFFER_INFOEX oldBuffer;
        LONG oldStyle;
        DWORD oldMode;
        CONSOLE_CURSOR_INFO oldCursorInfo;
        CONSOLE_FONT_INFOEX oldFont;
        int oldInTranslationMode;
        int oldOutTranslationMode;

        Navigation navi;
        Canvas canvas;
        Canvas::CanvasBuffer_t backup;  // Preserve a canvas buffer for popup,
                                        // make it more efficient

        const uint32_t targetFPS;
        const std::chrono::nanoseconds _targetFrameTime;

       public:
        Game(uint32_t fps = 60);
        void Init() override;
        void Run(const std::wstring_view screenName) override;

        AbstractGame* AddScreen(std::unique_ptr<AbstractScreen>) override;

        ~Game();
    };
}  // namespace ConsoleGame
