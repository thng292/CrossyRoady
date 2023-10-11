#pragma once

#include <array>

#include "../Common.h"

namespace ConsoleGame {
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