#pragma once

#include "Button.h"
#include "Common.h"
#include "ConsoleGame.h"

template <size_t N>
class HMenu {
    static constexpr int gap = 5;

    uint8_t lastHover = 0;

   public:
    uint8_t hover = 0;
    std::array<Button, N> buttons;

    void Init(
        ConsoleGame::Vec2 startPos,
        ConsoleGame::Vec2 buttonSize,
        std::array<const std::string_view, N> buttonLabels
    )
    {
        for (int i = 0, tmp = startPos.x; i < buttons.size();
             i++, tmp += buttonSize.width + gap) {
            buttons[i] = Button(
                {
                    .size = buttonSize,
                    .pos = {tmp, startPos.y},
                    .cornerSize = 5,
                    .hasBorder = true,
                    .background = (ConsoleGame::Color)14,
                    .border = (ConsoleGame::Color)13
            },
                buttonLabels[i],
                (ConsoleGame::Color)13,
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
                    (ConsoleGame::Color)14,
                    (ConsoleGame::Color)15,
                    (ConsoleGame::Color)13
                );
            } else {
                buttons[i].ChangeColor(
                    (ConsoleGame::Color)13,
                    (ConsoleGame::Color)14,
                    (ConsoleGame::Color)13
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
