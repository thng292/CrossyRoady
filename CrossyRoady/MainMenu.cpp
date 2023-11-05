#include "MainMenu.h"

#include "StringRes.h"

using namespace ConsoleGame;
using namespace std::literals;
using enum AbstractNavigation::NavigationAction;

void MainMenu::Init()
{
    menu.Init(
        startPos,
        buttDim,
        std::array<const std::string_view, 5>{
            StringRes::Get(StrRes::SinglePlayer),
            StringRes::Get(StrRes::TwoPlayer),
            StringRes::Get(StrRes::Progress),
            StringRes::Get(StrRes::Setting),
            StringRes::Get(StrRes::Exit)}
    );
}

void MainMenu::Mount() {}

AbstractNavigation::NavigationRes MainMenu::Update(
    float deltaTime,
    const AbstractNavigation* navigation,
    SubMenu& currentScreen

)
{
    AbstractNavigation::NavigationRes res = navigation->NoChange();
    menu.Update(
        deltaTime,
        [&](uint8_t selected) noexcept {
            if (*sfxOpt == StringRes::Get(StrRes::OnOpt)) {
                hoverSfx->Play();
            }
        },
        [&](uint8_t selection) noexcept {
            if (*sfxOpt == StringRes::Get(StrRes::OnOpt)) {
                PlayAndForget(RESOURCE_PATH SFX_PATH "select.wav");
            }
            switch (selection) {
                case 3:
                    currentScreen = SubMenu::Setting;
                    break;
                case 4:
                    res = navigation->Exit();
                    break;
            }
        }
    );
    return res;
}

void MainMenu::Draw(AbstractCanvas* canvas) const
{
    canvas->Clear((Color)6);
    Font::DrawString(canvas, "Crossy Roady", {10, 10}, 3, 1, (Color)13);
    menu.Draw(canvas);
}

void MainMenu::Unmount() {}
