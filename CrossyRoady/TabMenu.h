#pragma once

#include <concepts>
#include <functional>
#include <type_traits>

#include "Button.h"
#include "Common.h"
#include "ConsoleGame.h"
#include "Menu.h"

template <size_t N>
class TabMenu {
    static constexpr int gap = 5;

    uint8_t lastHover = 0;

   public:
    int16_t hover = 0;
    uint8_t selected = 0;
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
                {
                    .size = buttonSize,
                    .pos = {startPos.x, tmp},
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
        hover = -1;
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
            if (hover >= 0) {
                onSelectChange(hover);
            }
        }

        if (ConsoleGame::UiIsKeyMeanSelect()) {
            selected = hover;
            onTriggerCB(hover);
        } else if (ConsoleGame::UiIsKeyDown(VK_LBUTTON) 
            and hover >=0 and buttons[hover].IsHover(mousePos) )
        {
            selected = hover;
            onTriggerCB(hover);
        }

        for (int i = 0; i < buttons.size(); i++) {
            if (selected == i) {
                buttons[i].ChangeColor(
                    (ConsoleGame::Color)14,
                    (ConsoleGame::Color)13,
                    (ConsoleGame::Color)13
                );
            } else if (hover == i) {
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
