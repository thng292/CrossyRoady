#include "Common.h"

#include <array>
#include <bit>

#include "Logger.h"
#include "raylib.h"

namespace ConsoleGame {
    constexpr std::array inputFunc = {
        IsKeyMeanUp,
        IsKeyMeanDown,
        IsKeyMeanLeft,
        IsKeyMeanRight,
        IsKeyMeanSelect,
        IsKeyMeanEscape,
        IsKeyMeanBack,
        +[] { return IsKeyDown(MOUSE_LEFT_BUTTON); }
    };

    KeyState keyboardState[8] = {KeyState::Normal};
    bool isForeground         = true;
    Vec2 mousePos             = {0, 0};
    Vec2 canvasPixelSize      = {0, 0};

    void SetupInput() {}

    void GetInput()
    {
        auto pos = GetMousePosition();
        mousePos = Vec2{.x = int(pos.x / 3), .y = int(pos.y / 3)};
        for (int i = 0; i < inputFunc.size(); i++) {
            auto isDown = inputFunc[i]();
            if (isDown) {
                if (keyboardState[i] == KeyState::Normal) {
                    keyboardState[i] = KeyState::Pressed;
                } else {
                    keyboardState[i] = KeyState::Holding;
                }
            } else {
                if (keyboardState[i] == KeyState::Released) {
                    keyboardState[i] = KeyState::Normal;
                } else if (keyboardState[i] == KeyState::Holding) {
                    keyboardState[i] = KeyState::Released;
                } else {
                    keyboardState[i] = KeyState::Normal;
                }
            }
        }
    }

    bool IsWindowForeground() { return isForeground; }

    bool IsKeyMeanUp()
    {
        return (
            (IsKeyDown('W') or IsKeyDown(KEY_UP) or
             IsKeyDown(GAMEPAD_BUTTON_LEFT_FACE_UP))
        );
    }

    bool IsKeyMeanDown()
    {
        return (
            (IsKeyDown('S') or IsKeyDown(KEY_DOWN) or
             IsKeyDown(GAMEPAD_BUTTON_LEFT_FACE_DOWN))
        );
    }

    bool IsKeyMeanLeft()
    {
        return (
            (IsKeyDown('A') or IsKeyDown(KEY_LEFT) or
             IsKeyDown(GAMEPAD_BUTTON_LEFT_FACE_LEFT))
        );
    }

    bool IsKeyMeanRight()
    {
        return (
            (IsKeyDown('D') or IsKeyDown(KEY_RIGHT) or
             IsKeyDown(GAMEPAD_BUTTON_LEFT_FACE_RIGHT))
        );
    }

    bool IsKeyMeanSelect()
    {
        return (
            (IsKeyDown(KEY_ENTER) or IsKeyDown('F') or
             IsKeyDown(GAMEPAD_BUTTON_RIGHT_FACE_DOWN))
        );
    }

    bool IsKeyMeanBack()
    {
        return (IsKeyDown('B') or IsKeyDown(GAMEPAD_BUTTON_RIGHT_FACE_LEFT));
    }

    bool IsKeyMeanEscape()
    {
        return (
            IsKeyDown(KEY_ESCAPE) or IsKeyDown(GAMEPAD_BUTTON_MIDDLE_RIGHT)
        );
    }

    using enum KeyState;

    bool UiIsKeyMeanUp() { return keyboardState[0] == Pressed; }

    bool UiIsKeyMeanDown() { return keyboardState[1] == Pressed; }

    bool UiIsKeyMeanLeft() { return keyboardState[2] == Pressed; }

    bool UiIsKeyMeanRight() { return keyboardState[3] == Pressed; }

    bool UiIsKeyMeanSelect() { return keyboardState[4] == Pressed; }

    bool UiIsKeyMeanEscape() { return keyboardState[5] == Pressed; }

    bool UiIsKeyMeanBack() { return keyboardState[6] == Pressed; }

    bool UiIsKeyMeanClick() { return keyboardState[7] == Pressed; }

    Vec2 GetMousePos() { return mousePos; }

    auto GetDisplayRefreshRate() -> int
    {
        auto res = GetMonitorRefreshRate(GetCurrentMonitor());
        return res != 0 ? res : 60;
    }

}  // namespace ConsoleGame
