#include "Canvas.h"

#include <algorithm>

namespace ConsoleGame {

    Vec2 Canvas::CanvasSize() const { return _CanvasSize; }

    Color* Canvas::operator[](size_t index)
    {
        return canvasBuffer.data() + index * _CanvasSize.width;
    }

    void Canvas::Clear(Color color)
    {
        std::fill(canvasBuffer.begin(), canvasBuffer.end(), color);
    }

    Canvas::Canvas()
        : screenBuffer(_ScreenSize.width * _ScreenSize.height, 0),
          canvasBuffer(
              _CanvasSize.width * _CanvasSize.height, Color::BRIGHT_WHITE
          ){};

    void Canvas::Init(HANDLE handleOut)
    {
        _handleOut = handleOut;
        DWORD charWritten = 0;
        FillConsoleOutputAttribute(
            _handleOut,
            0,
            _ScreenSize.width * _ScreenSize.height,
            {0, 0},
            &charWritten
        );
        FillConsoleOutputCharacterW(
            _handleOut,
            L'\u2584',  // Unicode Character "Lower Half block" (U+2584)
            _ScreenSize.width * _ScreenSize.height,
            {0, 0},
            &charWritten
        );
    }

    void Canvas::DrawToScreen()
    {
        const auto rowSize = _ScreenSize.width;
        for (size_t i = 0; i < _ScreenSize.height; i++) {
            size_t upper = 2 * i;
            size_t lower = upper + 1;
            for (size_t j = 0; j < _ScreenSize.width; j++) {
                // Change the background for the upper row
                // Change the foreground for the lower row
                // & 15 to handle the transparent case.
                // transparent on the last layer is BRIGHT_WHITE
                screenBuffer[i * rowSize + j] =
                    ((char)canvasBuffer[upper * rowSize + j] << 4) |
                    ((char)canvasBuffer[lower * rowSize + j] & 0b1111);
            }
        }
        DWORD written = 0;
        WriteConsoleOutputAttribute(
            _handleOut,
            screenBuffer.data(),
            screenBuffer.size(),
            {0, 0},
            &written
        );
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
