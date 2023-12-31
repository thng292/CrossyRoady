#include "Game.h"

#include <fcntl.h>
#include <io.h>

#include <algorithm>
#include <array>
#include <cstdarg>
#include <format>
#include <iostream>
#include <system_error>
#include <thread>

#include "Common.h"
#include "Palette.h"
#include "Signal.h"

 //#define _ENABLE_ASYNC_DRAW_
constexpr bool SHOW_FPS          = true;
constexpr bool SHOULD_SKIP_FRAME = true;

namespace ConsoleGame {
    void SetupInput(HWND);
    void GetInput();

    Game::Game(const std::wstring_view& winName, uint32_t fps)
        : targetFPS(fps),
          _targetFrameTime(
              std::chrono::nanoseconds(std::chrono::seconds(1)) / targetFPS
          ),
          windowName(winName)
    {
        hStdOut     = GetStdHandle(STD_OUTPUT_HANDLE);
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

        consoleWindow = GetConsoleWindow();
        LONG style    = GetWindowLong(consoleWindow, GWL_STYLE);
        oldStyle      = style;

        // Turn off maximize, resize, horizontal and vertical scrolling
        style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME) & ~(WS_HSCROLL) &
                ~(WS_VSCROLL);
        SetWindowLong(consoleWindow, GWL_STYLE, style);

        // Turn off mouse input
        DWORD currMode;
        GetConsoleMode(hGameScreen, &currMode);
        oldMode = currMode;
        err     = SetConsoleMode(
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
        oldCursorInfo       = cursorInfo;

        cursorInfo.bVisible = false;
        err                 = SetConsoleCursorInfo(hGameScreen, &cursorInfo);
        debugError(err);

        CONSOLE_FONT_INFOEX fontex;
        // Set font bold
        fontex.cbSize = sizeof(CONSOLE_FONT_INFOEX);
        err           = GetCurrentConsoleFontEx(hGameScreen, 0, &fontex);
        debugError(err);
        oldFont             = fontex;

        // A character width is now 3 pixel
        fontex.dwFontSize.X = 6;
        fontex.dwFontSize.Y = 6;
        wcscpy_s(fontex.FaceName, L"Consolas");
        err = SetCurrentConsoleFontEx(hGameScreen, NULL, &fontex);
        debugError(err);

        // Update console title
        SetConsoleTitle(windowName.data());

        // Set IO Unicode
        oldOutTranslationMode = _setmode(_fileno(stdout), _O_WTEXT);
        oldInTranslationMode  = _setmode(_fileno(stdin), _O_WTEXT);

        // Set BufferSize
        err                   = SetConsoleScreenBufferSize(
            hGameScreen, {_ScreenSize.width, _ScreenSize.height}
        );
        debugError(err);

        // Change color palette
        oldBuffer.cbSize = sizeof(oldBuffer);
        err              = GetConsoleScreenBufferInfoEx(hStdOut, &oldBuffer);
        debugError(err);

        system(std::format(
                   "MODE CON COLS={} LINES={}",
                   _ScreenSize.width,
                   _ScreenSize.height
        )
                   .c_str());

    }

    void Game::Run(std::wstring_view screenName)
    {
        SetupInput(consoleWindow);
        canvas.Init(hGameScreen);
        canvas.Clear();

        HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);

        naviStack.reserve(screens.size());
        naviStack.emplace_back(screens[screenName]->Clone());
        auto navigationRes = navi.NoChange();

        // OS scheduler delay
        timeBeginPeriod(1);
        defer { timeEndPeriod(1); };

        float deltaTime  = 0;
        bool lastWasBack = false;
        using clock      = std::chrono::steady_clock;

        auto DrawFunc    = [&] { canvas.DrawToScreen(); };

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

        defer
        {
            DrawingThread.request_stop();
            startDrawSignal.StartJob();
        };

#endif

        while (!naviStack.empty()) {
            const auto& currentScreen = naviStack.back();
            if (lastWasBack) {
                currentScreen->Mount(navigationRes.Payload);
            } else {
                currentScreen->Init(navigationRes.Payload);
                currentScreen->Mount(std::any());
            }
            navigationRes = navi.NoChange();

            auto start    = clock::now();
            while (navigationRes.ActionType ==
                   AbstractNavigation::NavigationAction::None) {
                if constexpr (SHOW_FPS) {
                    SetConsoleTitle(
                        std::format(
                            L"{} - FPS: {}", windowName, 1.0f / deltaTime
                        )
                            .c_str()
                    );
                }

                GetInput();

                navigationRes = currentScreen->Update(deltaTime, &navi);
                currentScreen->Draw(&canvas);

#ifdef _ENABLE_ASYNC_DRAW_
                if constexpr (SHOULD_SKIP_FRAME) {
                    // Signal to draw
                    // Skip a frame if draw takes lots of time
                    if (startDrawSignal.JobDone()) {
                        startDrawSignal.StartJob();
                    }
                } else {
                    startDrawSignal.WaitUntilJobDone();
                    startDrawSignal.StartJob();
                }
#else
                DrawFunc();
#endif
                constexpr auto secondToNano =
                    std::chrono::nanoseconds(std::chrono::seconds(1)).count();

                constexpr auto OS_SchedulerDelay = std::chrono::milliseconds(2);
                const auto nextFrame =
                    start + _targetFrameTime - OS_SchedulerDelay;
                std::this_thread::sleep_until(nextFrame);

                const auto now = clock::now();

                deltaTime      = float((now - start).count()) / secondToNano;
                start          = now;
            }  // Out of screen's loop

#ifdef _ENABLE_ASYNC_DRAW_
            startDrawSignal.WaitUntilJobDone();
#endif

            naviStack.back()->Unmount();
            switch (navigationRes.ActionType) {
                case AbstractNavigation::NavigationAction::Back:
                    naviStack.pop_back();
                    lastWasBack = true;
                    break;
                case AbstractNavigation::NavigationAction::PopBackTo:
                    for (auto i = naviStack.size() - 1; i >= 0; i--) {
                        if (naviStack[i]->getName() ==
                            navigationRes.ActionData) {
                            while (naviStack.size()  - 1 != i) {
                                naviStack.pop_back();
                            }
                            break;
                        }
                    }
                    lastWasBack = true;
                    break;
                case AbstractNavigation::NavigationAction::Navigate:
                    if (screens.contains(navigationRes.ActionData)) {
                        naviStack.emplace_back(
                            screens[navigationRes.ActionData]->Clone()
                        );
                    }
                    lastWasBack = false;
                    break;
                case AbstractNavigation::NavigationAction::Exit:
                    return;
            }
        }
    }

    Game* Game::AddScreen(std::unique_ptr<AbstractScreen> screen)
    {
        auto name     = screen->getName();
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
        system(std::format(
                   "MODE CON COLS={} LINES={}",
                   oldBuffer.dwSize.X,
                   oldBuffer.dwSize.Y
        )
                   .c_str());
    }

}  // namespace ConsoleGame
