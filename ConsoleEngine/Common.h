#pragma once

#include <Windows.h>

#undef min
#undef max

#include <bit>
#include <concepts>
#include <cstdint>
#include <memory>

#include "Defer.h"

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
    constexpr int _CONSOLE_WIDTH_ = 384;
    constexpr int _CONSOLE_HEIGHT_ = 112;

    constexpr Vec2 _ScreenSize{
        .width = _CONSOLE_WIDTH_, .height = _CONSOLE_HEIGHT_};

    // Ratio: 25:14
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
    int GetDisplayRefreshRate();

    template <std::unsigned_integral T>
    T HostToBigEndian(T num)
    {
        if constexpr (std::endian::native == std::endian::big) {
            return num;
        } else {
            union {
                uint8_t bytes_rep[sizeof(T)];
                T num_rep;
            } tmp{};

            tmp.num_rep = num;
            for (int i = 0, j = sizeof(T) - 1; i < j; i++, j--) {
                uint8_t ttt = tmp.bytes_rep[i];
                tmp.bytes_rep[i] = tmp.bytes_rep[j];
                tmp.bytes_rep[j] = ttt;
            }
            return tmp.num_rep;
        }
    }

    template <std::unsigned_integral T>
    T BigEndianToHost(T num)
    {
        return HostToBigEndian(num);
    }
}  // namespace ConsoleGame
