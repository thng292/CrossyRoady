#include "Common.h"

#include <bit>

namespace ConsoleGame {
    KeyState keyboardState[8] = {KeyState::Normal};
    bool isForeground         = true;
    Vec2 mousePos             = {0, 0};

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

    bool UiIsKeyMeanUp() { return keyboardState[0] == Released; }

    bool UiIsKeyMeanDown() { return keyboardState[1] == Released; }

    bool UiIsKeyMeanLeft() { return keyboardState[2] == Released; }

    bool UiIsKeyMeanRight() { return keyboardState[3] == Released; }

    bool UiIsKeyMeanSelect() { return keyboardState[4] == Released; }

    bool UiIsKeyMeanEscape() { return keyboardState[5] == Released; }

    bool UiIsKeyMeanBack() { return keyboardState[6] == Released; }

    bool UiIsKeyMeanClick() { return keyboardState[7] == Released; }

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
