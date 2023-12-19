#pragma once

#include <bit>
#include <concepts>
#include <cstdint>
#include <memory>
#include <string_view>

#include "Defer.h"
#include "raylib.h"

#define RGB(r, g, b) (Color{r, g, b, 255})

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

    struct Box {
        Vec2 coord;
        Vec2 dim;
    };

    static constexpr int ColorPaletteSize = 16;
    using ColorPalette_t                  = std::array<Color, ColorPaletteSize>;

    const char C_TRANSPARENT              = 31;

    constexpr int _CONSOLE_WIDTH_         = 384;
    constexpr int _CONSOLE_HEIGHT_        = 112;
    constexpr int PORT                    = 6969;
    constexpr std::string_view PORT_STR   = "6969";

    constexpr Vec2 _ScreenSize{
        .width = _CONSOLE_WIDTH_, .height = _CONSOLE_HEIGHT_
    };

    // Ratio: 25:14
    constexpr Vec2 _CanvasSize{
        .width = _ScreenSize.width, .height = 2 * _ScreenSize.height
    };

    enum class KeyState : uint8_t { Normal, Pressed, Holding, Released };

    bool IsWindowForeground();
    // Get KeyCode:
    // https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
    bool IsKeyMeanUp();
    bool IsKeyMeanDown();
    bool IsKeyMeanLeft();
    bool IsKeyMeanRight();
    bool IsKeyMeanSelect();
    bool IsKeyMeanEscape();
    bool IsKeyMeanBack();

    bool UiIsKeyMeanUp();
    bool UiIsKeyMeanDown();
    bool UiIsKeyMeanLeft();
    bool UiIsKeyMeanRight();
    bool UiIsKeyMeanSelect();
    bool UiIsKeyMeanEscape();
    bool UiIsKeyMeanBack();
    bool UiIsKeyMeanClick();

    Vec2 GetMousePos();
    int GetDisplayRefreshRate();

    template <std::integral T>
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
                uint8_t ttt      = tmp.bytes_rep[i];
                tmp.bytes_rep[i] = tmp.bytes_rep[j];
                tmp.bytes_rep[j] = ttt;
            }
            return tmp.num_rep;
        }
    }

    template <std::integral T>
    T BigEndianToHost(T num)
    {
        return HostToBigEndian(num);
    }
}  // namespace ConsoleGame
