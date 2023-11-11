#include "Control.h"

#include <format>
using namespace ConsoleGame;

const std::wstring_view Control::ScreenName() { return L"Control"; }

std::wstring_view Control::getName() { return ScreenName(); }

void Control::Init(const std::any& args)
{
    test = Button(
        SurfaceArgs{
            .size = {60,  22},
            .pos = {150, 80},
            .cornerSize = 4,
            .background = (Color)14,
            .border = (Color)13,
    },
        "Test",
        ((Color)13),
        1,
        1
    );
}

ConsoleGame::AbstractScreen* Control::Clone() const { return new Control; }

ConsoleGame::AbstractNavigation::NavigationRes Control::Update(
    float deltaTime, const ConsoleGame::AbstractNavigation* navigation
)
{
    return navigation->NoChange();
}

void Control::Draw(ConsoleGame::AbstractCanvas* canvas) const
{
    test.Draw(canvas);
}