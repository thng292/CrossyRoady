#pragma once

#include "Common.h"
#include "Surface.h"

class ArrowButton : private Surface {
    bool direction;
    float count = 0;

   public:
    ArrowButton() = default;
    ArrowButton(SurfaceArgs surfaceArgs, bool isRight);

    bool IsHover(ConsoleGame::Vec2 mousePos) const;
    void Draw(ConsoleGame::AbstractCanvas* canvas) const;

    void Mount() { count = 0; }

    void ChangeColor(
        ConsoleGame::Color backgroundColor, ConsoleGame::Color borderColor
    );

    void Update(float deltaTime, auto onHover, auto onClick)
    {
        if (count <= buttonDelay) {
            count += deltaTime;
        }
        auto KeyFunc = ConsoleGame::IsKeyMeanLeft;
        if (direction) {
            KeyFunc = ConsoleGame::IsKeyMeanRight;
        }
        auto tmp = IsHover(ConsoleGame::GetMousePos());
        if (tmp) {
            onHover();
        }
        if (((ConsoleGame::IsKeyMeanSelect() and tmp) or KeyFunc()) and
            count > buttonDelay) {
            count = 0;
            onClick();
        }
    }
};