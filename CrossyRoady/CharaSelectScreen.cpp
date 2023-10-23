#include "CharaSelectScreen.h"

#include <algorithm>

using namespace std::literals;
using namespace ConsoleGame;

const std::wstring_view CharaSelectScreen::ScreenName()
{
    return L"CharaSelectScreen"sv;
}

std::wstring_view CharaSelectScreen::getName() { return ScreenName(); }

void CharaSelectScreen::Init(const std::any& args)
{
    bg.Load(fileName);
    palette.Load(paletteName);
    ChangeColorPalette(palette);
}

AbstractScreen* CharaSelectScreen::Clone() const
{
    return new CharaSelectScreen(*this);
}

AbstractNavigation::NavigationRes CharaSelectScreen::Update(
    float deltaTime, const AbstractNavigation* navigation
)
{
    return navigation->NoChange();
}

void CharaSelectScreen::Draw(AbstractCanvas* canvas) const
{
    bg.Paint(canvas, DRAW_COORD);
}