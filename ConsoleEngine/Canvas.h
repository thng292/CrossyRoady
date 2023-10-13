#pragma once
#include <array>

#include "Abstract/AbstractCanvas.h"

namespace ConsoleGame {
    class Canvas final : public AbstractCanvas {
        HANDLE _handleOut;
        ScreenBuffer_t screenBuffer;
        CanvasBuffer_t canvasBuffer;

       public:

        Canvas();
        // Inherited via AbstractCanvas
        void Init(HANDLE handleOut) override;
        Vec2 CanvasSize() const override;
        const CanvasBuffer_t& ReadCanvas() const override;
        void WriteCanvas(const CanvasBuffer_t buffer) override;
        Color* operator[](size_t index) override;
        void Clear(Color color = Color::BRIGHT_WHITE) override;
        void DrawToScreen() override;
    };
}  // namespace ConsoleGame
