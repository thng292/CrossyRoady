#include "AniScreen.h"

using namespace ConsoleGame;
using namespace std::literals;

const std::wstring_view AniScreen::ScreenName() { return L"AniScreen"sv; }

std::wstring_view AniScreen::getName() { return ScreenName(); }

void AniScreen::Init(const std::any& args) { animation.Load("out.anisprite"); }

AbstractScreen* AniScreen::Clone() const { return new AniScreen; }

AbstractNavigation::NavigationRes AniScreen::Update(
    float deltaTime, const AbstractNavigation* navigation
)
{
    if (!animation.IsPlaying()) {
        animation.Play();
    } else {
        animation.AutoUpdateFrame(deltaTime);
    }
    return navigation->NoChange();
}

void AniScreen::Draw(AbstractCanvas* canvas) const
{
    animation.Paint(canvas, {0,0});
}
