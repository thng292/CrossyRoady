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
            .height = (windowRect.bottom - windowRect.top) / _CanvasSize.height};
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

    void ChangeColorPalette(const ColorPalette& palette) {
        static const auto hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFOEX buffer{0};
        GetConsoleScreenBufferInfoEx(hStdOut, &buffer);
        buffer.cbSize = sizeof(buffer);

        for (int i = 0; i < _DefaultColorPalette.size(); i++) {
            buffer.ColorTable[i] = _DefaultColorPalette[i];
        }

        SetConsoleScreenBufferInfoEx(hStdOut, &buffer);


    }

}  // namespace ConsoleGame
