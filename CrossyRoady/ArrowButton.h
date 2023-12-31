#pragma once

#include "Common.h"
#include "Surface.h"

class ArrowButton : public Surface {
    bool direction;
    bool lastHover = false;
    ConsoleGame::Color primaryColor = (ConsoleGame::Color)13;
    ConsoleGame::Color secondaryColor = (ConsoleGame::Color)15;
    ConsoleGame::Color tertiaryColor = (ConsoleGame::Color)14;

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
        auto isHover = IsHover(ConsoleGame::GetMousePos());
        if (isHover) {
            if (!lastHover) {
                onHover();
            }
            Surface::props.background = primaryColor;
            lastHover = true;
        } else {
            Surface::props.background = tertiaryColor;
            lastHover = false;
        }
        if ((ConsoleGame::UiIsKeyMeanClick() and isHover) or KeyFunc()) {
            onClick();
        }
    }
};