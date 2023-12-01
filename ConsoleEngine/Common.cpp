#include "Common.h"

#include <array>
#include <bit>

#include "Logger.h"

namespace ConsoleGame {
    constexpr std::array inputFunc = {
        IsKeyMeanUp,
        IsKeyMeanDown,
        IsKeyMeanLeft,
        IsKeyMeanRight,
        IsKeyMeanSelect,
        IsKeyMeanEscape,
        IsKeyMeanBack,
        +[] { return IsKeyDown(VK_LBUTTON); }};

    KeyState keyboardState[8] = {KeyState::Normal};
    bool isForeground         = true;
    Vec2 mousePos             = {0, 0};
    Vec2 canvasPixelSize      = {0, 0};
    HWND consoleWindow        = 0;

    void SetupInput(HWND window)
    {
        consoleWindow = window;
        RECT windowRect;
        GetWindowRect(window, &windowRect);
        canvasPixelSize = {
            .width = (windowRect.right - windowRect.left) / _CanvasSize.width,
            .height =
                (windowRect.bottom - windowRect.top) / _CanvasSize.height};
    }

    void GetInput()
    {
        isForeground = GetForegroundWindow() == consoleWindow;
        if (isForeground) {
            POINT pos{0};
            GetCursorPos(&pos);
            ScreenToClient(consoleWindow, &pos);
            mousePos = Vec2{
                .x = pos.x / canvasPixelSize.width,
                .y = pos.y / canvasPixelSize.height};
        }
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

    bool ConsoleGame::IsKeyDown(int key)
    {
        return (GetAsyncKeyState(key) & 0x8000) and isForeground;
    }

    bool IsKeyMeanUp()
    {
        return ((GetAsyncKeyState('W') | GetAsyncKeyState(VK_UP) |
                 GetAsyncKeyState(VK_GAMEPAD_DPAD_UP) |
                 GetAsyncKeyState(VK_GAMEPAD_LEFT_THUMBSTICK_UP)) &
                0x8000) &&
               isForeground;
    }

    bool IsKeyMeanDown()
    {
        return ((GetAsyncKeyState('S') | GetAsyncKeyState(VK_DOWN) |
                 GetAsyncKeyState(VK_GAMEPAD_DPAD_DOWN) |
                 GetAsyncKeyState(VK_GAMEPAD_LEFT_THUMBSTICK_DOWN)) &
                0x8000) &&
               isForeground;
    }

    bool IsKeyMeanLeft()
    {
        return ((GetAsyncKeyState('A') | GetAsyncKeyState(VK_LEFT) |
                 GetAsyncKeyState(VK_GAMEPAD_DPAD_LEFT) |
                 GetAsyncKeyState(VK_GAMEPAD_LEFT_THUMBSTICK_LEFT)) &
                0x8000) &&
               isForeground;
    }

    bool IsKeyMeanRight()
    {
        return ((GetAsyncKeyState('D') | GetAsyncKeyState(VK_RIGHT) |
                 GetAsyncKeyState(VK_GAMEPAD_DPAD_RIGHT) |
                 GetAsyncKeyState(VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT)) &
                0x8000) &&
               isForeground;
    }

    bool IsKeyMeanSelect()
    {
        return ((GetAsyncKeyState(VK_RETURN) | GetAsyncKeyState('F') |
                 GetAsyncKeyState(VK_GAMEPAD_A)) &
                0x8000) and
               isForeground;
    }

    bool IsKeyMeanBack()
    {
        return ((GetAsyncKeyState('B') | GetAsyncKeyState(VK_GAMEPAD_B)) &
                0x8000) and
               isForeground;
    }

    bool IsKeyMeanEscape()
    {
        return ((GetAsyncKeyState(VK_ESCAPE) | GetAsyncKeyState(VK_GAMEPAD_MENU)
                ) &
                0x8000) and
               isForeground;
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

    Vec2 ConsoleGame::GetMousePos() { return mousePos; }

    auto GetDisplayRefreshRate() -> int
    {
        DEVMODE devMode;
        devMode.dmSize = sizeof(devMode);
        bool ok = EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode);
        if (ok) {
            return devMode.dmDisplayFrequency;
        }
        return 60;
    }

}  // namespace ConsoleGame
