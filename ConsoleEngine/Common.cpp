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

    static constexpr bool isHostBigEndian =
        std::endian::native == std::endian::big;

    uint16_t BigEndianToHost(uint16_t num)
    {
        if (!isHostBigEndian) {
            return ((num >> 8)) | ((num << 8));
        } else {
            return num;
        }
    }

    uint16_t HostToBigEndian(uint16_t num)
    {
        if (!isHostBigEndian) {
            return num;
        } else {
            return (num << 8) | (num >> 8);
        }
    }
    union uint32 {
        uint32_t num;
        uint8_t bytes[4];
    };

    uint32_t BigEndianToHost(uint32_t num)
    {
        if (isHostBigEndian) {
            return num;
        } else {
            uint32 tmp;
            tmp.num = num;
            std::swap(tmp.bytes[0], tmp.bytes[3]);
            std::swap(tmp.bytes[1], tmp.bytes[2]);
            return tmp.num;
        }
    }
}  // namespace ConsoleGame
