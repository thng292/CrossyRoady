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
        +[] { return IsMouseButtonDown(MOUSE_LEFT_BUTTON); }
    };

    KeyState keyboardState[8] = {KeyState::Normal};
    bool isForeground         = true;
    Vec2 mousePos             = {0, 0};
    Vec2 canvasPixelSize      = {0, 0};
    float leftStickX          = 0.0f;
    float leftStickY          = 0.0f;

    void SetupInput() {}

    void GetInput()
    {
        auto pos = GetMousePosition();
        const auto screen_width = (float)GetRenderWidth();
        const auto screen_height = (float)GetRenderHeight();
        const auto scale = std::min(
            screen_width / _CanvasSize.width,
            screen_height / _CanvasSize.height
        );
        const auto scaled_width = _CanvasSize.width * scale;
        const auto scaled_height = _CanvasSize.height * scale;
        const auto blackbar_x = (screen_width - scaled_width) / 2;
        const auto blackbar_y = (screen_height - scaled_height) / 2;

        mousePos = Vec2{
            .x = int((pos.x - blackbar_x) / scaled_width * _CanvasSize.width), 
            .y = int((pos.y - blackbar_y) / scaled_height * _CanvasSize.height)
        };
        const int gamepad = 0;
        leftStickX = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_X);
        leftStickY = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_Y);

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
             IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_UP) or
             leftStickY > 0.1f)
        );
    }

    bool IsKeyMeanDown()
    {
        return (
            (IsKeyDown('S') or IsKeyDown(KEY_DOWN) or
             IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN) or
             leftStickY < -0.1f)
        );
    }

    bool IsKeyMeanLeft()
    {
        return (
            (IsKeyDown('A') or IsKeyDown(KEY_LEFT) or
             IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT) or
             leftStickX < -0.1f)
        );
    }

    bool IsKeyMeanRight()
    {
        return (
            (IsKeyDown('D') or IsKeyDown(KEY_RIGHT) or
             IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT) or
             leftStickX > 0.1f)
        );
    }

    bool IsKeyMeanSelect()
    {
        return (
            (IsKeyDown(KEY_ENTER) or IsKeyDown('F') or
             IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN))
        );
    }

    bool IsKeyMeanBack()
    {
        return (
            IsKeyDown('B') or
            IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_LEFT)
        );
    }

    bool IsKeyMeanEscape()
    {
        return (
            IsKeyDown(KEY_ESCAPE) or
            IsGamepadButtonDown(0, GAMEPAD_BUTTON_MIDDLE_RIGHT)
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
