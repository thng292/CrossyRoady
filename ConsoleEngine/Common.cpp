#include "Common.h"

#include <bit>

namespace ConsoleGame {

    static const auto wind = GetConsoleWindow();

    bool IsWindowForeground() { return wind == GetForegroundWindow(); }

    bool ConsoleGame::IsKeyDown(int key)
    {
        return (GetAsyncKeyState(key) & (1 << 16)) &&
               (IsWindowForeground());
    }

    bool IsKeyMeanUp()
    {
        return ((GetAsyncKeyState('W') | GetAsyncKeyState(VK_UP) |
                 GetAsyncKeyState(VK_GAMEPAD_DPAD_UP) |
                 GetAsyncKeyState(VK_GAMEPAD_LEFT_THUMBSTICK_UP)) &
                (1 << 16)) &&
               (IsWindowForeground());
    }

    bool IsKeyMeanDown()
    {
        return ((GetAsyncKeyState('S') | GetAsyncKeyState(VK_DOWN) |
                 GetAsyncKeyState(VK_GAMEPAD_DPAD_DOWN) |
                 GetAsyncKeyState(VK_GAMEPAD_LEFT_THUMBSTICK_DOWN)) &
                (1 << 16)) &&
               (IsWindowForeground());
    }

    bool IsKeyMeanLeft()
    {
        return ((GetAsyncKeyState('A') | GetAsyncKeyState(VK_LEFT) |
                 GetAsyncKeyState(VK_GAMEPAD_DPAD_LEFT) |
                 GetAsyncKeyState(VK_GAMEPAD_LEFT_THUMBSTICK_LEFT)) &
                (1 << 16)) &&
               (IsWindowForeground());
    }

    bool IsKeyMeanRight()
    {
        return ((GetAsyncKeyState('D') | GetAsyncKeyState(VK_RIGHT) |
                 GetAsyncKeyState(VK_GAMEPAD_DPAD_RIGHT) |
                 GetAsyncKeyState(VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT)) &
                (1 << 16)) &&
               (IsWindowForeground());
    }

    bool IsKeyMeanSelect()
    {
        return ((GetAsyncKeyState(VK_RETURN) | GetAsyncKeyState('F') |
                 GetAsyncKeyState(VK_GAMEPAD_A)) &
                (1 << 16)) &&
               (IsWindowForeground());
    }

    bool IsKeyMeanBack()
    {
        return ((GetAsyncKeyState('B') | GetAsyncKeyState(VK_GAMEPAD_B)) &
                (1 << 16)) &&
               (IsWindowForeground());
    }

    bool IsKeyMeanEscape()
    {
        return ((GetAsyncKeyState(VK_ESCAPE) | GetAsyncKeyState(VK_GAMEPAD_MENU)) &
                (1 << 16)) &&
               (IsWindowForeground());
    }
    Vec2 getCanvasPixelSize(HWND hConsoleWindow)
    {
        RECT windowRect;
        GetWindowRect(hConsoleWindow, &windowRect);
        return Vec2{
            .width = (windowRect.right - windowRect.left) / _CanvasSize.width,
            .height =
                (windowRect.bottom - windowRect.top) / _CanvasSize.height};
    }

    Vec2 ConsoleGame::GetMousePos()
    {
        static const Vec2 pixSize = getCanvasPixelSize(wind);
        POINT pos{0};
        GetCursorPos(&pos);
        ScreenToClient(wind, &pos);
        return Vec2{.x = pos.x / pixSize.width, .y = pos.y / pixSize.height};
    }

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
