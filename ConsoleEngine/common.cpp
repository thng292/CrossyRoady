#include "Common.h"

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

    uint16_t ToHost(uint16_t num)
    {
        return ((num >> 8)) | ((num << 8));
    }

    uint16_t ToBigEndian(uint16_t num)
    {
        static bool isBigEndian = ToHost(500) == 500;
        if (isBigEndian) {
            return num;
        } else {
            return (num << 8) | (num >> 8);
        }
    }

}  // namespace ConsoleGame
