#include "Setting.h"

#include "StringRes.h"

using namespace ConsoleGame;

Setting::Setting()
{
    title = Button(
        {
            .size = buttDim,
            .pos = {startPos.x, 20},
            .cornerSize = 5,
            .hasBorder = true,
            .background = (Color)14,
            .border = (Color)13
    },
        StringRes::Get(StrRes::MusicToggle),
        (Color)13,
        0
    );
}

const std::wstring_view Setting::ScreenName() { return L"SettingScreen"; }

std::wstring_view Setting::getName() { return ScreenName(); }

void Setting::Init(const std::any& args) {}

AbstractScreen* Setting::Clone() const { return new Setting; }

AbstractNavigation::NavigationRes Setting::Update(
    float deltaTime, const AbstractNavigation* navigation
)
{
    return navigation->NoChange();
}

void Setting::Draw(AbstractCanvas* canvas) const
{
    title.Draw(canvas);
    for (const auto& butt : buttons) {
        butt.Draw(canvas);
    }
}
