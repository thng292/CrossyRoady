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
        canvasBuffer.resize(_CanvasSize.width * _CanvasSize.height);
    }

    void Canvas::DrawToScreen()
    {
        BeginDrawing();
        for (int y = 0; y < _CanvasSize.height; ++y) {
            for (int x = 0; x < _CanvasSize.width; ++x) {
                if (canvasBuffer[y * _CanvasSize.width + x] == 31) {
                    continue;
                }
                DrawRectangle(
                    x * 3,
                    y * 3,
                    3,
                    3,
                    currentColorPallete[canvasBuffer[y * _CanvasSize.width + x]]
                );
            }
        }

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
