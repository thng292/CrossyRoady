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
        textureBuffer.resize(_CanvasSize.width * _CanvasSize.height);
        image = { 0 };
        image.data = textureBuffer.data();
        image.width = _CanvasSize.width;
        image.height = _CanvasSize.height;
        image.mipmaps = 1;
        image.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8; // Most common format for Color arrays

        // Load the initial texture
        texture = LoadTextureFromImage(image);

    }

    void Canvas::DrawToScreen()
    {
        for (int y = 0; y < _CanvasSize.height; ++y) {
            for (int x = 0; x < _CanvasSize.width; ++x) {
                if (canvasBuffer[y * _CanvasSize.width + x] == 31) {
                    continue;
                }
                textureBuffer[y * _CanvasSize.width + x] = currentColorPallete[canvasBuffer[y * _CanvasSize.width + x]];
            }
        }
        UpdateTexture(texture, textureBuffer.data());

        BeginDrawing();
        //void DrawTexturePro(Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, Color tint);
        DrawTexturePro(texture, {0, 0, _CanvasSize.width, _CanvasSize.height}, {0, 0, (float)GetRenderWidth(), (float)GetRenderHeight()}, {0, 0}, 0, WHITE);
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
