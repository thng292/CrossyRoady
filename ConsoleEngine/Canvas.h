#pragma once

#include "Abstract/AbstractCanvas.h"
#include "Common.h"

namespace ConsoleGame {

    extern ColorPalette_t currentColorPallete;

    class Canvas final : public AbstractCanvas {
        std::vector<char> canvasBuffer;

       public:
        // Inherited via AbstractCanvas
        void Init() override;
        Vec2 CanvasSize() const override;
        const CanvasBuffer_t& ReadCanvas() const override;
        void WriteCanvas(CanvasBuffer_t buffer) override;
        char* operator[](size_t index) override;
        void Clear(char color = 0) override;
        void DrawToScreen() override;
    };
}  // namespace ConsoleGame
