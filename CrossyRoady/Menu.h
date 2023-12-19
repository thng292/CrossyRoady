#pragma once

#include "Button.h"
#include "Common.h"
#include "ConsoleGame.h"

static void forVSintelisense(uint8_t sel) noexcept {};

template <size_t N>
class Menu {
    static constexpr int gap = 5;
    uint8_t lastHover = 0;

   public:
    char primaryColor = (char)13;
    char secondaryColor = (char)15;
    char tertiaryColor = (char)14;
    uint8_t hover = 0;
    std::array<Button, N> buttons;

    void Init(
        ConsoleGame::Vec2 startPos,
        ConsoleGame::Vec2 buttonSize,
        std::array<const std::string_view, N> buttonLabels
    )
    {
        for (int i = 0, tmp = startPos.y; i < buttons.size();
             i++, tmp += buttonSize.height + gap) {
            buttons[i] = Button(
                {.size = buttonSize,
                 .pos = {startPos.x, tmp},
                 .cornerSize = 5,
                 .hasBorder = true,
                 .background = tertiaryColor,
                 .border = primaryColor},
                buttonLabels[i],
                primaryColor,
                0
            );
        }
    }

    template <MenuSelectedCB Func1, MenuSelectedCB Func2>
    void Update(float deltaTime, Func1 onSelectChange, Func2 onTriggerCB)
    {
        if (ConsoleGame::UiIsKeyMeanDown()) {
            hover = (hover + 1 + buttons.size()) % buttons.size();
        }
        if (ConsoleGame::UiIsKeyMeanUp()) {
            hover = (hover - 1 + buttons.size()) % buttons.size();
        }

        auto mousePos = ConsoleGame::GetMousePos();

        for (int i = 0; i < buttons.size(); i++) {
            if (buttons[i].IsHover(mousePos)) {
                hover = i;
            }
        }

        if (hover != lastHover) {
            lastHover = hover;
            onSelectChange(hover);
        }

        if (ConsoleGame::UiIsKeyMeanSelect()) {
            onTriggerCB(hover);
        } else if (ConsoleGame::UiIsKeyMeanClick() && buttons[hover].IsHover(mousePos)) {
            onTriggerCB(hover);
        }

        for (int i = 0; i < buttons.size(); i++) {
            if (hover == i) {
                buttons[i].ChangeColor(
                    tertiaryColor, secondaryColor, primaryColor
                );
            } else {
                buttons[i].ChangeColor(
                    primaryColor, tertiaryColor, primaryColor
                );
            }
        }
    }

    void Draw(ConsoleGame::AbstractCanvas* canvas) const
    {
        for (const auto& butt : buttons) {
            butt.Draw(canvas);
        }
    }
};
