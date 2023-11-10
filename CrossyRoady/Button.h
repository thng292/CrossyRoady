#pragma once
#include <string_view>

#include "Surface.h"

class Button : private Surface {
    std::string_view text;
    ConsoleGame::Color textColor;
    ConsoleGame::Vec2 textPos;
    uint8_t fontVariant;
    uint8_t fontSize;

    void calculateTextPos();

   public:
    Button() = default;
    Button(
        SurfaceArgs surfaceArgs,
        const std::string_view text,
        ConsoleGame::Color textColor,
        uint8_t fontVariant = 0,
        uint8_t fontSize = 1
    );

    void ChangeTextColor(ConsoleGame::Color textColor);
    bool IsHover(ConsoleGame::Vec2 mousePos) const;
    void ChangeColor(
        ConsoleGame::Color textColor,
        ConsoleGame::Color backgroundColor,
        ConsoleGame::Color borderColor
    );
    void ChangeText(const std::string_view text);
    void Draw(ConsoleGame::AbstractCanvas* canvas) const;
};
