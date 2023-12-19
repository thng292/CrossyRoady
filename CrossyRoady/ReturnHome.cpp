#include "ReturnHome.h"

using namespace ConsoleGame;

const std::wstring_view ReturnHome::ScreenName() { return L"ReturnHome"; }

std::wstring_view ReturnHome::getName() { return ScreenName(); }

void ReturnHome::Init(const std::any& args)
{
    menu.Init({127, 80}, {130, 18}, {R.String.Yes, R.String.No, R.String.Back});
    title = Button(
        {.size = {150, 24},
         .pos = {117, 30},
         .cornerSize = 5,
         .hasBorder = true,
         .background = (char)14,
         .border = (char)13},
        R.String.Pause.SaveGame,
        (char)13,
        1
    );
}

AbstractScreen* ReturnHome::Clone() const { return new ReturnHome; }

AbstractNavigation::NavigationRes ReturnHome::Update(
    float deltaTime, const AbstractNavigation* navigation
)
{
    auto nav = navigation->NoChange();
    menu.Update(
        deltaTime,
        [&](uint8_t hovering) noexcept { audio.PlayHoverSfx(); },
        [&](uint8_t selected) noexcept {
            audio.PlayClickSfx();
            switch (selected) {
                case 0:
                    nav = navigation->Navigate(L"Yes");
                    break;
                case 1:
                    nav = navigation->Navigate(L"No");
                    break;
                case 2:
                    nav = navigation->Back();
                    break;
            }
        }
    );
    if (IsKeyMeanBack()) {
        nav = navigation->Back();
    }
    return nav;
}

void ReturnHome::Draw(AbstractCanvas* canvas) const
{
    title.Draw(canvas);
    menu.Draw(canvas);
}
