#include "Common.h"

#include <bit>

namespace ConsoleGame {

    bool ConsoleGame::IsKeyDown(int key)
    {
        return (GetAsyncKeyState(key) & (1 << 16));
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
        static const HWND hConsoleWindow = GetConsoleWindow();
        static const Vec2 pixSize = getCanvasPixelSize(hConsoleWindow);
        POINT pos{0};
        GetCursorPos(&pos);
        ScreenToClient(hConsoleWindow, &pos);
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
