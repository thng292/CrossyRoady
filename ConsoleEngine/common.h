#pragma once

#include <Windows.h>

#include <cstdint>
#include <memory>

#ifndef _CONSOLE_WIDTH_
#define _CONSOLE_WIDTH_ 384
#endif

#ifndef _CONSOLE_HEIGHT_
#define _CONSOLE_HEIGHT_ 112
#endif

// Ratio: 25:14

namespace ConsoleGame {

    // clang-format off
    union Vec2 {
        struct { int x, y; };
        struct { int u, v; };
        struct { int width, height; };
        struct { int left, right; };
        int element[2];
    };

    // clang-format on

    constexpr Vec2 _ScreenSize{
        .width = _CONSOLE_WIDTH_, .height = _CONSOLE_HEIGHT_};
    constexpr Vec2 _CanvasSize{
        .width = _ScreenSize.width, .height = 2 * _ScreenSize.height};
    enum class Color : char {
        BLACK = 0,
        BLUE = 1,
        GREEN = 2,
        CYAN = 3,
        RED = 4,
        MAGENTA = 5,
        YELLOW = 6,
        WHITE = 7,
        GRAY = 8,
        LIGHT_BLUE = 9,
        LIGHT_GREEN = 10,
        LIGHT_CYAN = 11,
        LIGHT_RED = 12,
        LIGHT_MAGENTA = 13,
        LIGHT_YELLOW = 14,
        BRIGHT_WHITE = 15,
        C_TRANSPARENT = 0b11111
    };

    // Get KeyCode:
    // https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
    bool IsKeyDown(int keyCode);

    Vec2 GetMousePos();

    uint16_t HostToBigEndian(uint16_t num);
    uint32_t BigEndianToHost(uint32_t num);
    uint16_t BigEndianToHost(uint16_t num);
}  // namespace ConsoleGame
