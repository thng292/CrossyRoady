#include "Canvas.h"

#include <algorithm>
#include <array>

#include "raylib.h"

namespace ConsoleGame {
    ColorPalette_t currentColorPallete;

    Vec2 Canvas::CanvasSize() const { return _CanvasSize; }

    char* Canvas::operator[](size_t index)
    {
        return canvasBuffer.data() + index * _CanvasSize.width;
    }

    void Canvas::Clear(char color)
    {
        std::fill(canvasBuffer.begin(), canvasBuffer.end(), color);
    }

    void Canvas::Init()
    {
        toScreen =
            GenImageColor(_CanvasSize.width * 3, _CanvasSize.height * 3, BLACK);
        canvasBuffer.resize(_CanvasSize.width * _CanvasSize.height);
    }

    void Canvas::DrawToScreen()
    {
        BeginDrawing();
        // Loop through the canvasBuffer and draw each character to the screen

        EndDrawing();
    }

    const Canvas::CanvasBuffer_t& Canvas::ReadCanvas() const
    {
        return canvasBuffer;
    }

    void Canvas::WriteCanvas(const CanvasBuffer_t buffer)
    {
        std::copy(buffer.begin(), buffer.end(), canvasBuffer.begin());
    }
}  // namespace ConsoleGame
