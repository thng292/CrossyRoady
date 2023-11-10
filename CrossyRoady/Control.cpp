#include <format>

#include "Control.h"
using namespace ConsoleGame;

const std::wstring_view Control::ScreenName() { return L"Control"; }

std::wstring_view Control::getName() { return ScreenName(); }

void Control::Init(const std::any& args) {}

ConsoleGame::AbstractScreen* Control::Clone() const
{
    return new Control;
}

ConsoleGame::AbstractNavigation::NavigationRes Control::Update(
    float deltaTime, const ConsoleGame::AbstractNavigation* navigation
)
{
    return navigation->NoChange();
}

void Control::Draw(ConsoleGame::AbstractCanvas* canvas) const
{
    Font::DrawString(canvas, std::format("None"), {150, 70}, 1, 0);
}