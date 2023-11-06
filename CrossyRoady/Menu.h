#pragma once

#include <concepts>
#include <functional>
#include <type_traits>

#include "Button.h"
#include "ConsoleGame.h"

static void forVSintelisense(uint8_t sel) noexcept {};

template <typename Func>
concept MenuSelectedCB = std::is_nothrow_invocable_r_v<void, Func, uint8_t>;

template <size_t N>
class Menu {
    static constexpr float buttonDelay = 1.0f / 5;
    static constexpr int gap = 5;

    std::array<Button, N> buttons;
    uint8_t selected = 0;
    uint8_t lastSelected = 0;
    float timePassed = 0;
    bool lastIsUp = false;

   public:
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
    void Update(
        float deltaTime,
        Func1 onSelectChange,
        Func2 onTriggerCB
    )
    {
        timePassed += deltaTime;
        if (ConsoleGame::IsKeyMeanDown()) {
            if (timePassed > buttonDelay || lastIsUp) {
                lastIsUp = false;
                timePassed = 0;
                selected = (selected + 1 + buttons.size()) % buttons.size();
            }
        }
        if (ConsoleGame::IsKeyMeanUp()) {
            if (timePassed > buttonDelay || !lastIsUp) {
                lastIsUp = true;
                timePassed = 0;
                selected = (selected - 1 + buttons.size()) % buttons.size();
            }
        }

        auto mousePos = ConsoleGame::GetMousePos();

        for (int i = 0; i < buttons.size(); i++) {
            if (buttons[i].IsHover(mousePos)) {
                selected = i;
            }
        }

        if (selected != lastSelected) {
            lastSelected = selected;
            onSelectChange(selected);
        }

        if (ConsoleGame::IsKeyMeanSelect() ||
            ConsoleGame::IsKeyDown(VK_LBUTTON)) {
            onTriggerCB(selected);
        }

        for (int i = 0; i < buttons.size(); i++) {
            if (selected == i) {
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
