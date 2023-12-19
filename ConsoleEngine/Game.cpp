#include "Game.h"

#include <thread>

#include "Common.h"
#include "raylib.h"

// #define _ENABLE_ASYNC_DRAW_
constexpr bool SHOW_FPS          = true;
constexpr bool SHOULD_SKIP_FRAME = true;

namespace ConsoleGame {
    void SetupInput();
    void GetInput();

    Game::Game(const std::string_view& winName, uint32_t fps)
        : targetFPS(fps),
          _targetFrameTime(
              std::chrono::nanoseconds(std::chrono::seconds(1)) / targetFPS
          ),
          windowName(winName)
    {
        InitWindow(_CanvasSize.width, _CanvasSize.height, winName.data());
        InitAudioDevice();
        SetTargetFPS(fps);
    }

    void Game::Run(std::wstring_view screenName)
    {
        SetupInput();
        canvas.Init();
        canvas.Clear();

        naviStack.reserve(screens.size());
        naviStack.emplace_back(screens[screenName]->Clone());
        auto navigationRes = navi.NoChange();

        // OS scheduler delay

        float deltaTime    = 0;
        bool lastWasBack   = false;
        using clock        = std::chrono::steady_clock;

        auto DrawFunc      = [&] { canvas.DrawToScreen(); };

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
                if (WindowShouldClose()) {
                    return;
                }
                GetInput();

                navigationRes = currentScreen->Update(deltaTime, &navi);
                currentScreen->Draw(&canvas);

                DrawFunc();
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
                            while (naviStack.size() - 1 != i) {
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
        CloseAudioDevice();
        CloseWindow();
    }

}  // namespace ConsoleGame
