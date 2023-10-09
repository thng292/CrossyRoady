#pragma once

#include <Windows.h>

#include <cstdint>
#include <memory>

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

    struct InputRecords {
        static constexpr int MaxSize = 10;
        INPUT_RECORD buffer[MaxSize];
        DWORD size = 0;

        INPUT_RECORD* begin() const { return (INPUT_RECORD*)buffer; }

        INPUT_RECORD* end() const { return (INPUT_RECORD*)buffer + size; }
        
    };

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
        C_TRANSPARENT = 16
    };
}  // namespace ConsoleGame
