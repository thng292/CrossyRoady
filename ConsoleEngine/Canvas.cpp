#include "Canvas.h"

namespace ConsoleGame {

    Vec2 Canvas::CanvasSize() const { return _CanvasSize; }

    Color* Canvas::operator[](size_t index)
    {
        return canvasBuffer.data() + index * _CanvasSize.width;
    }

    void Canvas::Clear(Color color) { canvasBuffer.fill(color); }

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
        for (int i = 0; i < 30; i++) {
            int upper = 2 * i;
            int lower = upper + 1;
            for (int j = 0; j < 120; j++) {
                // Change the background for the upper row
                // Change the foreground for the lower row
                screenBuffer[i * rowSize + j] =
                    (char(canvasBuffer[upper * rowSize + j]) << 4) |
                    char(canvasBuffer[lower * rowSize + j]);
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
