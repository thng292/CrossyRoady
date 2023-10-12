#pragma once

#include <Windows.h>

#include <array>
#include <cstdint>
#include <memory>

#ifndef _CONSOLE_WIDTH_
#define _CONSOLE_WIDTH_ 120
#endif

#ifndef _CONSOLE_HEIGHT_
#define _CONSOLE_HEIGHT_ 30
#endif

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

    using ColorPalette = std::array<COLORREF, 16>;

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

    constexpr ColorPalette _DefaultColorPalette = {
        RGB(12, 12, 12),
        RGB(0, 55, 218),
        RGB(19, 161, 14),
        RGB(58, 150, 221),
        RGB(197, 15, 31),
        RGB(136, 23, 152),
        RGB(193, 156, 0),
        RGB(204, 204, 204),
        RGB(118, 118, 118),
        RGB(59, 120, 255),
        RGB(22, 198, 12),
        RGB(97, 214, 214),
        RGB(231, 72, 86),
        RGB(180, 0, 158),
        RGB(249, 241, 165),
        RGB(242, 242, 242),
    };

    // Get KeyCode:
    // https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
    bool IsKeyDown(int keyCode);

    Vec2 GetMousePos();

    void ChangeColorPalette(const ColorPalette& palette);

}  // namespace ConsoleGame
