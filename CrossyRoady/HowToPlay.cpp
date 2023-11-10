#include "HowToPlay.h"

#include "Button.h"
#include "Control.h"
#include "Exp.h"
#include "StringRes.h"
#include "Upgrade.h"
using namespace ConsoleGame;

const std::wstring_view HowToPlay::ScreenName() { return L"HowToPlay"; }

std::wstring_view HowToPlay::getName() { return ScreenName(); }

void HowToPlay::Init(const std::any& args)
{
    bg.Init();
    Title = Button(
        {
            .size = {250,             22},
            .pos = {(384 - 250) / 2, 10},
            .cornerSize = 7,
            .hasBorder = true,
            .background = (Color)14,
            .border = ((Color)13)
    },
        R.HowToPlay.Title,
        ((Color)13),
        1,
        1
    );
    surface = Surface({
        .size = {245, 160},
        .pos = {125, 45 },
        .cornerSize = 7,
        .hasBorder = true,
        .background = (Color)14,
        .border = ((Color)13)
    });
    menu.Init(
        {10, 80},
        {100, 18},
        std::array<const std::string_view, 4>{
            R.HowToPlay.Control,
            R.HowToPlay.Exp,
            R.HowToPlay.Upgrade,
            R.Back}
    );

    subScreen[0] = std::make_unique<Control>();
    subScreen[1] = std::make_unique<Exp>();
    subScreen[2] = std::make_unique<Upgrade>();
}

ConsoleGame::AbstractScreen* HowToPlay::Clone() const { return new HowToPlay; }

ConsoleGame::AbstractNavigation::NavigationRes HowToPlay::Update(
    float deltaTime, const ConsoleGame::AbstractNavigation* navigation
)
{
    bg.Update(deltaTime);
    auto res = navigation->NoChange();
    menu.Update(
        deltaTime,
        [&](uint8_t hovering) noexcept {},
        [&](uint8_t selected) noexcept {
            switch (selected) {
                case 0:
                    break;
                case 1:
                    break;
                case 2:
                    break;
                case 3:
                    res = navigation->Back();
                    break;
            }
        }
    );
    return res;
}

void HowToPlay::Draw(ConsoleGame::AbstractCanvas* canvas) const
{
    bg.Draw(canvas);
    Title.Draw(canvas);
    surface.Draw(canvas);
    menu.Draw(canvas);
    subScreen[menu.selected]->Draw(canvas);
}