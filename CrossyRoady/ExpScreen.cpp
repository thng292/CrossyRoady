#include "ExpScreen.h"

using namespace ConsoleGame;

const std::wstring_view ExpScreen::ScreenName() { return L"Exp"; }

std::wstring_view ExpScreen::getName() { return ScreenName(); }

void ExpScreen::Init(const std::any& args)
{
    hoverSfx = std::any_cast<Audio*>(args);
    surface.props = {
        .size = {284, 204},
        .pos = {90,  10 },
        .cornerSize = 5,
        .hasBorder = true,
        .background = (Color)14,
        .border = (Color)13
    };
}

AbstractScreen* ExpScreen::Clone() const { return nullptr; }

AbstractNavigation::NavigationRes ExpScreen::Update(
    float deltaTime, const AbstractNavigation* navigation
)
{
    return navigation->NoChange();
}

void ExpScreen::Draw(AbstractCanvas* canvas) const { surface.Draw(canvas); }
