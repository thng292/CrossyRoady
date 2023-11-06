#include "Progress.h"

#include "Common.h"
#include "ExpScreen.h"
#include "Statistic.h"
#include "StringRes.h"

using namespace ConsoleGame;

const std::wstring_view Progress::ScreenName() { return L"Progress"; }

std::wstring_view Progress::getName() { return ScreenName(); }

void Progress::Init(const std::any& args)
{
    auto tmp = std::any_cast<SharedMenuStuff>(args);
    bg = tmp.menuBg;
    hoverSfx = tmp.hoverSfx;
    bgMusic = tmp.bgMusic;
    sfxOpt = (std::string*)&LocalStorage::Get(R.Config.SfxToggle);

    menu.Init(
        {5, 10},
        {80, 18},
        {R.Statistic.Title, R.Exp.Title, R.CharInfo.Title, R.Back}
    );

    subScreen[0] = std::make_unique<Statistic>();
    subScreen[1] = std::make_unique<ExpScreen>();
    for (auto& scr : subScreen) {
        scr->Init(hoverSfx);
    }
}

void Progress::Mount(const std::any& args)
{
    menu.selected = 0;
    for (auto& scr : subScreen) {
        scr->Mount(std::any());
    }
}

AbstractScreen* Progress::Clone() const { return new Progress; }

AbstractNavigation::NavigationRes Progress::Update(
    float deltaTime, const AbstractNavigation* navigation
)
{
    auto res = navigation->NoChange();
    bg->Update(deltaTime);
    subScreen[menu.selected]->Update(deltaTime, navigation);
    menu.Update(
        deltaTime,
        [&](uint8_t hovering) noexcept {
            if (*sfxOpt == R.Config.OnOpt) {
                hoverSfx->Play();
            }
        },
        [&](uint8_t selected) noexcept {
            if (*sfxOpt == R.Config.OnOpt) {
                PlayAndForget(RESOURCE_PATH SFX_PATH "select.wav");
            }
            switch (selected) {
                case 0:
                case 1:
                    oldSelected = selected;
                    break;
                case 2:
                    res = navigation->Navigate(L"sldfj");
                    menu.selected = oldSelected;
                    break;
                case 3:
                    res = navigation->Back();
                    menu.selected = oldSelected;
                    break;
            }
        }
    );
    return res;
}

void Progress::Draw(AbstractCanvas* canvas) const
{
    bg->Draw(canvas);
    menu.Draw(canvas);
    subScreen[menu.selected]->Draw(canvas);
}

void Progress::Unmount()
{
    for (auto& scr : subScreen) {
        scr->Unmount();
    }
}
