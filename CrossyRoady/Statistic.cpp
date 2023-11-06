#include "Statistic.h"

#include "StringRes.h"
#include "TimePlayedTracker.h"

using namespace ConsoleGame;

const std::wstring_view Statistic::ScreenName() { return L"Stat"; }

std::wstring_view Statistic::getName() { return ScreenName(); }

void Statistic::Init(const std::any& args)
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

AbstractScreen* Statistic::Clone() const { return new Statistic; }

AbstractNavigation::NavigationRes Statistic::Update(
    float deltaTime, const AbstractNavigation* navigation
)
{
    return navigation->NoChange();
}

void Statistic::Draw(AbstractCanvas* canvas) const { surface.Draw(canvas); }
