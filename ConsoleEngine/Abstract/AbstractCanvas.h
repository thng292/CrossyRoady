#pragma once

#include <vector>

#include "../Common.h"

namespace ConsoleGame {
    class AbstractCanvas {
       public:
        using CanvasBuffer_t                             = std::vector<char>;

        virtual void Init()                              = 0;
        virtual Vec2 CanvasSize() const                  = 0;
        virtual const CanvasBuffer_t& ReadCanvas() const = 0;
        virtual void WriteCanvas(CanvasBuffer_t buffer)  = 0;
        virtual char* operator[](size_t index)           = 0;
        virtual void Clear(char color = 0)               = 0;
        virtual void DrawToScreen()                      = 0;
        virtual ~AbstractCanvas()                        = default;
    };
}  // namespace ConsoleGame