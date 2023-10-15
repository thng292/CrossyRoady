#include "DevScreen.h"

#include <conio.h>
#include <ctype.h>

#include <algorithm>

using namespace std::literals;
using namespace ConsoleGame;

const std::wstring_view DevScreen::ScreenName() { return L"DevScreen"sv; }

std::wstring_view DevScreen::getName() { return ScreenName(); }

void DevScreen::Init(const std::any& args) {}

AbstractScreen* DevScreen::Clone() const { return new DevScreen; }

AbstractNavigation::NavigationRes DevScreen::Update(
    float deltaTime, const AbstractNavigation* navigation
)
{
    if (IsKeyDown('W')) {
        fontSize += 1;
    }
    if (IsKeyDown('S')) {
        fontSize -= 1;
        fontSize = std::max(fontSize, 0);
    }
    // if (IsKeyDown('W')) {
    //     moveY = max(moveY - 5, 0);
    // }
    // if (IsKeyDown('S')) {
    //     moveY = min(moveY + 5, _CanvasSize.height - dim.height);
    // }
    // if (IsKeyDown('A')) {
    //     moveX = max(moveX - 5, 0);
    // }
    // if (IsKeyDown('D')) {
    //     moveX = min(moveX + 5, _CanvasSize.width - dim.width);
    // }
    return navigation->NoChange();
}

void DevScreen::Draw(AbstractCanvas* canvas) const
{
    // for (int i = 0; i < dim.width; i++) {
    //     for (int j = 0; j < dim.height; j++) {
    //         (*canvas)[i + moveY][j + moveX] = Color::YELLOW;
    //     }
    // }
    Font::DrawString(canvas, "Hello A New World!", {-20, 50}, fontSize);
}
