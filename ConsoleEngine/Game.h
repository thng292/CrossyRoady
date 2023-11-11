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

        HWND consoleWindow;
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
        const std::wstring_view windowName;

        Vec2 canvasPixelSize;

        void UpdateMousePos();
       public:
        Game(const std::wstring_view& winName, uint32_t fps = 60);
        ~Game();
        void Run(const std::wstring_view screenName);

        Game* AddScreen(std::unique_ptr<AbstractScreen> screen);
    };
}  // namespace ConsoleGame
