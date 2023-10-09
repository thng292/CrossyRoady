#include "DevScreen.h"

using namespace std::literals;
using namespace ConsoleGame;

std::wstring_view DevScreen::getName() { return L"DevScreen"sv; }

void DevScreen::Init(const std::any& args) {}

AbstractScreen* DevScreen::Clone() const { return new DevScreen; }

AbstractNavigation::NavigationRes DevScreen::Update(
    const InputRecords& inputs,
    float deltaTime,
    const AbstractNavigation* navigation
)
{
    for (const auto& input : inputs) {
        if (input.EventType == KEY_EVENT) {
            switch (input.Event.KeyEvent.uChar.AsciiChar) {
                case 'w':
                    moveY = max(moveY - 2, 0);
                    break;
                case 's':
                    moveY = min(moveY + 2, _CanvasSize.height - dim.height);
                    break;
                case 'a':
                    moveX = max(moveX - 2, 0);
                    break;
                case 'd':
                    moveX = min(moveX + 2, _CanvasSize.width - dim.width);
                    break;
            }
        }
    }
    return navigation->NoChange();
}

void DevScreen::Draw(AbstractCanvas* canvas) const
{
    for (int i = 0; i < dim.width; i++) {
        for (int j = 0; j < dim.height; j++) {
            (*canvas)[i + moveY][j + moveX] = Color::YELLOW;
        }
    }
}

void DevScreen::DeInit() {}
