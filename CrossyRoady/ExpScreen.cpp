#include "ExpScreen.h"

using namespace ConsoleGame;

const std::wstring_view ExpScreen::ScreenName() { return L"Exp"; }

std::wstring_view ExpScreen::getName() { return ScreenName(); }

void ExpScreen::Init(const std::any& args)
{
    hoverSfx = std::any_cast<Audio*>(args);
}

AbstractScreen* ExpScreen::Clone() const { return nullptr; }

AbstractNavigation::NavigationRes ExpScreen::Update(
    float deltaTime, const AbstractNavigation* navigation
)
{
    return navigation->NoChange();
}

void ExpScreen::Draw(AbstractCanvas* canvas) const
{
    Font::DrawString(canvas, R.Exp.Title, {50, 50});
}
