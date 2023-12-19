#pragma once
#include <string_view>

#include "Surface.h"

class Button : public Surface {
    std::string_view text;
    char textColor;
    ConsoleGame::Vec2 textPos;
    uint8_t fontVariant;
    uint8_t fontSize;

    void calculateTextPos();

   public:
    Button() = default;
    Button(
        SurfaceArgs surfaceArgs,
        const std::string_view text,
        char textColor,
        uint8_t fontVariant = 0,
        uint8_t fontSize = 1
    );

    void ChangeTextColor(char textColor);
    bool IsHover(ConsoleGame::Vec2 mousePos) const;
    void ChangeColor(char textColor, char backgroundColor, char borderColor);
    void ChangeText(const std::string_view text);
    void Draw(ConsoleGame::AbstractCanvas* canvas) const;
};
