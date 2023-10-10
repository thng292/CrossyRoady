#pragma once

#include <array>

#include "../Common.h"

#ifndef _CONSOLE_WIDTH_
#define _CONSOLE_WIDTH_ 120
#endif

#ifndef _CONSOLE_HEIGHT_
#define _CONSOLE_HEIGHT_ 30 
#endif


namespace ConsoleGame {
    constexpr Vec2 _ScreenSize{.width = _CONSOLE_WIDTH_, .height = _CONSOLE_HEIGHT_};
    constexpr Vec2 _CanvasSize{.width = _ScreenSize.width, .height = 2 * _ScreenSize.height};

    class AbstractCanvas {
       public:
        using ScreenBuffer_t =
            std::array<WORD, _ScreenSize.width * _ScreenSize.height>;
        using CanvasBuffer_t =
            std::array<Color, _CanvasSize.width * _CanvasSize.height>;
        virtual void Init(HANDLE handleOut) = 0;
        virtual Vec2 CanvasSize() const = 0;
        virtual const CanvasBuffer_t& ReadCanvas() const = 0;
        virtual void WriteCanvas(const CanvasBuffer_t buffer) = 0;
        virtual Color* operator[](size_t index) = 0;
        virtual void Clear(Color color = Color::BRIGHT_WHITE) = 0;
        virtual void DrawToScreen() = 0;
    };
}  // namespace ConsoleGame