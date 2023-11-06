#include "Statistic.h"

#include "StringRes.h"
#include "TimePlayedTracker.h"

using namespace ConsoleGame;

const std::wstring_view Statistic::ScreenName() { return L"Stat"; }

std::wstring_view Statistic::getName() { return ScreenName(); }

void Statistic::Init(const std::any& args)
{
    hoverSfx = std::any_cast<Audio*>(args);

    titleButt = Button(
        {
            .size = {120,                           24},
            .pos = {(_CanvasSize.width - 100) / 2, 40},
            .cornerSize = 5,
            .hasBorder = true,
            .background = (Color)14,
            .border = (Color)13
    },
        R.Statistic.Title,
        (Color)13,
        1
    );

    surface.props = {
        .size = {100,                           100                           },
        .pos = {(_CanvasSize.width - 100) / 2, (_CanvasSize.height - 100) / 2},
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
    auto res = navigation->NoChange();
    return res;
}

void Statistic::Draw(AbstractCanvas* canvas) const
{
    titleButt.Draw(canvas);
    surface.Draw(canvas);
}
