#include "Common.h"

#include <bit>

namespace ConsoleGame {
    KeyState keyboardState[8] = {KeyState::Normal};
    bool isForeground           = true;
    Vec2 mousePos               = {0, 0};

    bool IsWindowForeground() { return isForeground; }

    bool ConsoleGame::IsKeyDown(int key)
    {
        return (GetAsyncKeyState(key) & 0x8000) and isForeground;
    }

    bool ConsoleGame::UiIsKeyDown(int key)
    {
        return keyboardState[key] == KeyState::Released and isForeground;
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

    bool UiIsKeyMeanUp()
    {
        return (keyboardState['W'] == Released or
                keyboardState[VK_UP] == Released or
                keyboardState[VK_GAMEPAD_DPAD_UP] == Released or
                keyboardState[VK_GAMEPAD_LEFT_THUMBSTICK_UP] == Released) and
               isForeground;
    }

    bool UiIsKeyMeanDown()
    {
        return (keyboardState['S'] == Released or
                keyboardState[VK_DOWN] == Released or
                keyboardState[VK_GAMEPAD_DPAD_DOWN] == Released or
                keyboardState[VK_GAMEPAD_LEFT_THUMBSTICK_DOWN] == Released) and
               isForeground;
    }

    bool UiIsKeyMeanLeft()
    {
        return (keyboardState['A'] == Released or
                keyboardState[VK_LEFT] == Released or
                keyboardState[VK_GAMEPAD_DPAD_LEFT] == Released or
                keyboardState[VK_GAMEPAD_LEFT_THUMBSTICK_LEFT] == Released) and
               isForeground;
    }

    bool UiIsKeyMeanRight()
    {
        return (keyboardState['D'] == Released or
                keyboardState[VK_RIGHT] == Released or
                keyboardState[VK_GAMEPAD_DPAD_RIGHT] == Released or
                keyboardState[VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT] == Released) &&
               isForeground;
    }

    bool UiIsKeyMeanSelect()
    {
        return (keyboardState[VK_RETURN] == Released or
                keyboardState['F'] == Released or
                keyboardState[VK_GAMEPAD_A] == Released) and
               isForeground;
    }

    bool UiIsKeyMeanBack()
    {
        return (keyboardState['B'] == Released or
                keyboardState[VK_GAMEPAD_B] == Released) and
               isForeground;
    }

    bool UiIsKeyMeanEscape()
    {
        return (keyboardState[VK_ESCAPE] == Released or
                keyboardState[VK_GAMEPAD_MENU] == Released) and
               isForeground;
    }

    bool UiIsKeyMeanClick()
    {
        return keyboardState[VK_LBUTTON] == Released and isForeground;
    }

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
