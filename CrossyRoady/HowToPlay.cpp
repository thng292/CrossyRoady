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
    if (args.has_value()) {
        bg = std::any_cast<MenuBG*>(args);
    }
    Title = Button(
        {.size = {250, 22},
         .pos = {(384 - 250) / 2, 10},
         .cornerSize = 7,
         .hasBorder = true,
         .background = (char)14,
         .border = ((char)13)},
        R.String.HowToPlay.Title,
        ((char)13),
        1,
        1
    );
    surface = Surface(
        {.size = {245, 160},
         .pos = {125, 45},
         .cornerSize = 7,
         .hasBorder = true,
         .background = (char)14,
         .border = ((char)13)}
    );
    menu.Init(
        {10, 80},
        {100, 18},
        std::array<const std::string_view, 4>{
            R.String.HowToPlay.Control,
            R.String.HowToPlay.Exp,
            R.String.HowToPlay.Upgrade,
            R.String.Back
        }
    );

    subScreen[0] = std::make_unique<Control>();
    subScreen[1] = std::make_unique<Exp>();
    subScreen[2] = std::make_unique<Upgrade>();
    for (auto& tmp : subScreen) {
        tmp->Init(std::any());
    }
}

ConsoleGame::AbstractScreen* HowToPlay::Clone() const { return new HowToPlay; }

ConsoleGame::AbstractNavigation::NavigationRes HowToPlay::Update(
    float deltaTime, const ConsoleGame::AbstractNavigation* navigation
)
{
    if (bg) {
        bg->Update(deltaTime);
    }
    auto res = navigation->NoChange();
    menu.Update(
        deltaTime,
        [&](uint8_t hovering) noexcept { audio.PlayHoverSfx(); },
        [&](uint8_t selected) noexcept {
            audio.PlayClickSfx();
            switch (selected) {
                case 0:
                    break;
                case 1:
                    break;
                case 2:
                    break;
                case 3:
                    menu.selected = 0;
                    res = navigation->Back(1);
                    break;
            }
        }
    );
    return res;
}

void HowToPlay::Draw(ConsoleGame::AbstractCanvas* canvas) const
{
    if (bg) {
        bg->Draw(canvas);
    }
    Title.Draw(canvas);
    surface.Draw(canvas);
    menu.Draw(canvas);
    subScreen[menu.selected]->Draw(canvas);
}