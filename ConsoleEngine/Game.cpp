#include "Game.h"

#include <fcntl.h>
#include <io.h>

#include <algorithm>
#include <cstdarg>
#include <iostream>
#include <system_error>
#include <thread>

#include "Signal.h"
#include "common.h"

#define _ENABLE_ASYNC_DRAW_

namespace ConsoleGame {
    void Game::Init()
    {
        hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
        hGameScreen = CreateConsoleScreenBuffer(
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL,
            CONSOLE_TEXTMODE_BUFFER,
            NULL
        );
        if (hStdOut == INVALID_HANDLE_VALUE ||
            hGameScreen == INVALID_HANDLE_VALUE) {
            throw std::system_category().message(GetLastError());
        }
        if (!SetConsoleActiveScreenBuffer(hGameScreen)) {
            throw std::system_category().message(GetLastError());
        }

        HWND consoleWindow = GetConsoleWindow();
        LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
        DWORD currMode;
        CONSOLE_FONT_INFOEX fontex;

        // Turn off maximize, resize, horizontal and vertical scrolling
        style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME) & ~(WS_HSCROLL) &
                ~(WS_VSCROLL);
        SetWindowLong(consoleWindow, GWL_STYLE, style);

        // Turn off mouse input
        GetConsoleMode(hGameScreen, &currMode);
        SetConsoleMode(
            hGameScreen,
            ((currMode | ENABLE_WINDOW_INPUT | ENABLE_EXTENDED_FLAGS) &
             ~ENABLE_QUICK_EDIT_MODE & ~ENABLE_MOUSE_INPUT)
        );

        // Hide scoll bar
        ShowScrollBar(consoleWindow, SB_BOTH, FALSE);

        // Hide the cursor
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(hGameScreen, &cursorInfo);
        cursorInfo.bVisible = false;
        SetConsoleCursorInfo(hGameScreen, &cursorInfo);

        // Set font bold
        fontex.cbSize = sizeof(CONSOLE_FONT_INFOEX);
        GetCurrentConsoleFontEx(hGameScreen, 0, &fontex);
        fontex.FontWeight = 800;
        fontex.dwFontSize.X = 24;
        fontex.dwFontSize.Y = 24;
        wcscpy_s(fontex.FaceName, L"Consolas");
        SetCurrentConsoleFontEx(hGameScreen, NULL, &fontex);

        // Update console title
        SetConsoleTitle(TEXT("Crossy Roady"));

        // Set IO Unicode
        _setmode(_fileno(stdout), _O_WTEXT);
        _setmode(_fileno(stdin), _O_WTEXT);

        backup.fill(Color::BRIGHT_WHITE);
    }

    void Game::Run(std::wstring_view screenName)
    {
        canvas.Init(hGameScreen);
        canvas.Clear();

        HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);

        naviStack.reserve(screens.size());
        naviStack.emplace_back(screens[screenName]->Clone(), false);
        AbstractNavigation::NavigationRes navigationRes{
            .ActionType = AbstractNavigation::NavigationAction::None,
            .Payload = std::any()};

        float deltaTime = 0;
        bool redraw = false;

        auto DrawFunc = [&] {
            const auto& currentScreen = naviStack.back();
            canvas.Clear();
            if (redraw) {
                const auto stackSize = naviStack.size() - 1;
                auto i = stackSize;
                while (naviStack[i].IsPopup) {
                    i--;
                }
                for (; i < stackSize; i++) {
                    naviStack[i].Screen->Draw(&canvas);
                }
                const auto& cbuff = canvas.ReadCanvas();
                std::copy(cbuff.begin(), cbuff.end(), backup.begin());
                redraw = false;
            } else if (currentScreen.IsPopup) {
                canvas.WriteCanvas(backup);
            }
            currentScreen.Screen->Draw(&canvas);
            canvas.DrawToScreen();
        };

#ifdef _ENABLE_ASYNC_DRAW_
        Signal startDrawSignal;

        auto DrawingThread = std::jthread([&](std::stop_token stoken) {
            while (1) {
                startDrawSignal.WaitStartJobSignal();
                if (!stoken.stop_requested()) {
                    DrawFunc();
                    startDrawSignal.DoneJob();
                } else {
                    return;
                }
            }
        });
#endif

        while (!naviStack.empty()) {
            const auto& currentScreen = naviStack.back();
            currentScreen.Screen->Init(navigationRes.Payload);
            navigationRes.ActionType =
                AbstractNavigation::NavigationAction::None;

            // Navigation will be delay 1 frame
            while (navigationRes.ActionType ==
                   AbstractNavigation::NavigationAction::None) {
                //
                const auto start = std::chrono::high_resolution_clock::now();
                navigationRes = currentScreen.Screen->Update(deltaTime, &navi);

#ifdef _ENABLE_ASYNC_DRAW_
                // Signal to draw
                // Skip a frame if draw takes lots of time
                if (shouldSkipFrame && startDrawSignal.JobDone()) {
                    startDrawSignal.StartJob();
                }
                if (!shouldSkipFrame) {
                    startDrawSignal.WaitUntilJobDone();
                    startDrawSignal.StartJob();
                }
#else
                DrawFunc();
#endif

                const auto nextFrame = start + _targetFrameTime;
                const auto now = std::chrono::high_resolution_clock::now();
                if (nextFrame < now) {
                    constexpr auto secondToNano =
                        std::chrono::nanoseconds(std::chrono::seconds(1))
                            .count();
                    deltaTime = float((now - start).count()) / secondToNano;
                } else {
                    deltaTime = targetFrameTime;
                    std::this_thread::sleep_until(nextFrame);
                }
            }

            switch (navigationRes.ActionType) {
                case AbstractNavigation::NavigationAction::Back:
                    currentScreen.Screen->DeInit();
                    naviStack.pop_back();
                    break;
                case AbstractNavigation::NavigationAction::PopBackTo:
                    for (auto it = naviStack.rbegin(), rend = naviStack.rend();
                         it != rend;
                         it++) {
                        if (it->Screen->getName() == navigationRes.ActionData) {
                            for (auto tmp = it.base(); tmp != naviStack.end();
                                 tmp++) {
                                tmp->Screen->DeInit();
                            }
                            naviStack.erase(it.base(), naviStack.end());
                        }
                    }
                    break;
                case AbstractNavigation::NavigationAction::Navigate:
                    if (screens.contains(navigationRes.ActionData)) {
                        naviStack.emplace_back(
                            screens[navigationRes.ActionData]->Clone(), false
                        );
                    }
                    break;
                case AbstractNavigation::NavigationAction::NavigatePopup:
                    if (screens.contains(navigationRes.ActionData)) {
                        naviStack.emplace_back(
                            screens[navigationRes.ActionData]->Clone(), true
                        );
                    }
                    break;
            }
            redraw = naviStack.back().IsPopup;
        }
#ifdef _ENABLE_ASYNC_DRAW_
        DrawingThread.request_stop();
        startDrawSignal.StartJob();
#endif
    }

    AbstractGame* Game::AddScreen(std::unique_ptr<AbstractScreen> screen)
    {
        auto name = screen->getName();
        screens[name] = std::move(screen);
        return this;
    }

    Game::~Game()
    {
        // Set IO Unicode
        _setmode(_fileno(stdout), _O_TEXT);
        _setmode(_fileno(stdin), _O_TEXT);
        if (!SetConsoleActiveScreenBuffer(hStdOut)) {
            std::cerr << std ::system_category().message(GetLastError());
        }
    }
}  // namespace ConsoleGame
