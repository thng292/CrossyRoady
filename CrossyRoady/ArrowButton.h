#pragma once

#include "Common.h"
#include "Surface.h"

class ArrowButton : private Surface {
    bool direction;
    bool lastHover = false;

   public:
    ArrowButton() = default;
    ArrowButton(SurfaceArgs surfaceArgs, bool isRight);

    bool IsHover(ConsoleGame::Vec2 mousePos) const;
    void Draw(ConsoleGame::AbstractCanvas* canvas) const;

    void Mount() {}

    void ChangeColor(
        ConsoleGame::Color backgroundColor, ConsoleGame::Color borderColor
    );

    void Update(float deltaTime, auto onHover, auto onClick)
    {
        auto KeyFunc = ConsoleGame::UiIsKeyMeanLeft;
        if (direction) {
            KeyFunc = ConsoleGame::UiIsKeyMeanRight;
        }
        auto tmp = IsHover(ConsoleGame::GetMousePos());
        if (tmp and not lastHover) {
            onHover();
            lastHover = true;
        } else {
            lastHover = false;
        }
        if ((ConsoleGame::UiIsKeyMeanClick() and tmp) or KeyFunc()) {
            onClick();
        }
    }
};