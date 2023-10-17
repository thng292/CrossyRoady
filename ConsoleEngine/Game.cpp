#include "Game.h"

#include <fcntl.h>
#include <io.h>

#include <algorithm>
#include <cstdarg>
#include <format>
#include <iostream>
#include <system_error>
#include <thread>

#include "Common.h"
#include "Palette.h"
#include "Signal.h"

#define _SHOW_FPS_
// #define _SHOULD_SKIP_FRAME_

#ifndef _DEBUG
#define _ENABLE_ASYNC_DRAW_
#endif

namespace ConsoleGame {

    Game::Game(uint32_t fps)
        : targetFPS(fps),
          _targetFrameTime(
              std::chrono::nanoseconds(std::chrono::seconds(1)) / targetFPS
          )
    {
    }

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
        bool err = 1;
        std::string errs;
        auto debugError = [&](bool err) {
            if (err == 0) {
                errs = std::system_category().message(GetLastError());
            }
        };
        if (hStdOut == INVALID_HANDLE_VALUE ||
            hGameScreen == INVALID_HANDLE_VALUE) {
            errs = std::system_category().message(GetLastError());
        }

        err = SetConsoleActiveScreenBuffer(hGameScreen);
        debugError(err);

        HWND consoleWindow = GetConsoleWindow();
        LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
        oldStyle = style;

        // Turn off maximize, resize, horizontal and vertical scrolling
        style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME) & ~(WS_HSCROLL) &
                ~(WS_VSCROLL);
        SetWindowLong(consoleWindow, GWL_STYLE, style);

        // Turn off mouse input
        DWORD currMode;
        GetConsoleMode(hGameScreen, &currMode);
        oldMode = currMode;
        err = SetConsoleMode(
            hStdOut, (currMode & (ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT))
        );
        debugError(err);

        // Hide scoll bar
        err = ShowScrollBar(consoleWindow, SB_BOTH, FALSE);
        debugError(err);

        // Hide the cursor
        CONSOLE_CURSOR_INFO cursorInfo;
        err = GetConsoleCursorInfo(hGameScreen, &cursorInfo);
        debugError(err);
        oldCursorInfo = cursorInfo;

        cursorInfo.bVisible = false;
        err = SetConsoleCursorInfo(hGameScreen, &cursorInfo);
        debugError(err);

        CONSOLE_FONT_INFOEX fontex;
        // Set font bold
        fontex.cbSize = sizeof(CONSOLE_FONT_INFOEX);
        err = GetCurrentConsoleFontEx(hGameScreen, 0, &fontex);
        debugError(err);
        oldFont = fontex;

        // A character width is now 3 pixel
        fontex.dwFontSize.X = 6;
        fontex.dwFontSize.Y = 6;
        wcscpy_s(fontex.FaceName, L"Consolas");
        err = SetCurrentConsoleFontEx(hGameScreen, NULL, &fontex);
        debugError(err);

        // Update console title
        SetConsoleTitle(TEXT("Crossy Roady"));

        // Set IO Unicode
        oldOutTranslationMode = _setmode(_fileno(stdout), _O_WTEXT);
        oldInTranslationMode = _setmode(_fileno(stdin), _O_WTEXT);

        // Set BufferSize
        err = SetConsoleScreenBufferSize(
            hGameScreen, {_ScreenSize.width, _ScreenSize.height}
        );
        debugError(err);

        // Change color palette
        oldBuffer.cbSize = sizeof(oldBuffer);
        err = GetConsoleScreenBufferInfoEx(hStdOut, &oldBuffer);
        debugError(err);

        auto newBuffer = oldBuffer;
        for (int i = 0; i < Palette::_DefaultColorPalette.size(); i++) {
            newBuffer.ColorTable[i] = Palette::_DefaultColorPalette[i];
        }
        err = SetConsoleScreenBufferInfoEx(hStdOut, &newBuffer);
        debugError(err);

        system(std::format(
                   "MODE CON COLS={} LINES={}",
                   _ScreenSize.width,
                   _ScreenSize.height
        )
                   .c_str());

        std::fill(backup.begin(), backup.end(), Color::BRIGHT_WHITE);
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

        // OS scheduler delay
        timeBeginPeriod(1);
        defer { timeEndPeriod(1); };
        float deltaTime = 0;
        bool redraw = false;
        using clock = std::chrono::steady_clock;

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

            auto start = clock::now();
            // Navigation will be delay 1 frame
            while (navigationRes.ActionType ==
                   AbstractNavigation::NavigationAction::None) {
#ifdef _SHOW_FPS_
                SetConsoleTitle(
                    std::format(L"Crossy Roady - FPS: {}", 1.0f / deltaTime)
                        .c_str()
                );
#endif

                navigationRes = currentScreen.Screen->Update(deltaTime, &navi);

#ifdef _ENABLE_ASYNC_DRAW_
#ifdef _SHOULD_SKIP_FRAME_
                // Signal to draw
                // Skip a frame if draw takes lots of time
                if (startDrawSignal.JobDone()) {
                    startDrawSignal.StartJob();
                }
#else
                startDrawSignal.WaitUntilJobDone();
                startDrawSignal.StartJob();
#endif
#else
                DrawFunc();
#endif
                constexpr auto secondToNano =
                    std::chrono::nanoseconds(std::chrono::seconds(1)).count();

                constexpr auto OS_SchedulerDelay = std::chrono::milliseconds(1);
                 const auto nextFrame = start + _targetFrameTime - OS_SchedulerDelay;
                 std::this_thread::sleep_until(nextFrame);

                const auto now = clock::now();

                /*auto dbgVal = float((now - nextFrame).count()) / 1'000'000;
                SetConsoleTitle(
                    std::format(L"Crossy Roady - FPS: {}", dbgVal).c_str()
                );*/

                deltaTime = float((now - start).count()) / secondToNano;
                start = now;
            }

            naviStack.back().Screen->Unmount();
            switch (navigationRes.ActionType) {
                case AbstractNavigation::NavigationAction::Back:
                    naviStack.pop_back();
                    break;
                case AbstractNavigation::NavigationAction::PopBackTo:
                    for (auto it = naviStack.rbegin(), rend = naviStack.rend();
                         it != rend;
                         it++) {
                        if (it->Screen->getName() == navigationRes.ActionData) {
                            for (auto tmp = it.base(); tmp != naviStack.end();
                                 tmp++) {
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
                case AbstractNavigation::NavigationAction::Exit:
                    return;
            }
            naviStack.back().Screen->Mount(navigationRes.Payload);
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
        _setmode(_fileno(stdin), oldInTranslationMode);
        _setmode(_fileno(stdout), oldOutTranslationMode);
        SetConsoleActiveScreenBuffer(hStdOut);
        SetConsoleScreenBufferInfoEx(hStdOut, &oldBuffer);
        SetCurrentConsoleFontEx(hStdOut, FALSE, &oldFont);
        SetWindowLong(GetConsoleWindow(), GWL_STYLE, oldStyle);
        SetConsoleMode(hStdOut, oldMode);
        SetConsoleCursorInfo(hStdOut, &oldCursorInfo);
    }

}  // namespace ConsoleGame
