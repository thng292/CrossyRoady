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

    void Update(float deltaTime, auto onHover, auto onClick)
    {
        if (count < buttonDelay) {
            count += deltaTime;
        }
        if (IsHover(ConsoleGame::GetMousePos())) {
            onHover();
            if (ConsoleGame::IsKeyMeanSelect() && count > buttonDelay) {
                onClick();
            }
        }
    }
};