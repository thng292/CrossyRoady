#include "Game.h"

#include <thread>
#include <cstdio>

#include "Common.h"
#include "Audio.h"
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
        SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI | FLAG_VSYNC_HINT);
        InitWindow(
            _CanvasSize.width * 3, _CanvasSize.height * 3, winName.data()
        );
        InitAudioDevice();
        SetExitKey(KEY_NULL);
    }

    void Game::Run(std::wstring_view screenName)
    {
        SetupInput();
        canvas.Init();
        canvas.Clear();

        naviStack.reserve(screens.size());
        naviStack.emplace_back(screens[screenName]->Clone());
        auto navigationRes = navi.NoChange();

        constexpr double wh_ratio = (double)_CanvasSize.width / _CanvasSize.height;

        bool lastWasBack   = false;

        auto DrawFunc      = [&] { canvas.DrawToScreen(); };

        auto screen_width_last = GetScreenWidth();
        auto screen_height_last = GetScreenHeight();

        while (!naviStack.empty()) {
            const auto& currentScreen = naviStack.back();
            if (lastWasBack) {
                currentScreen->Mount(navigationRes.Payload);
            } else {
                currentScreen->Init(navigationRes.Payload);
                currentScreen->Mount(std::any());
            }
            navigationRes = navi.NoChange();

            while (navigationRes.ActionType ==
                   AbstractNavigation::NavigationAction::None) {
                if (WindowShouldClose()) {
                    return;
                }
                GetInput();
                // if (UiIsKeyMeanClick()) {
                //     printf("Mouse: %d %d ", GetMousePos().x, GetMousePos().y);
                //     printf("Mouse Raw: %f %f ", GetMousePosition().x, GetMousePosition().y);
                //     printf("Screen: %d %d ", GetRenderWidth(), GetRenderHeight());
                //     printf("FPS: %d\n", GetFPS());
                // }
                for (auto instance: MusicStream::instances) {
                    instance->UpdateStreamBuffer();
                }

                navigationRes = currentScreen->Update(GetFrameTime(), &navi);
                currentScreen->Draw(&canvas);

                DrawFunc();
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
