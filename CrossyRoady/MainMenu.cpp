#include "MainMenu.h"

#include "StringRes.h"

using namespace ConsoleGame;
using namespace std::literals;
using enum AbstractNavigation::NavigationAction;

const std::wstring_view MainMenu::ScreenName() { return L"Main menu"; }

std::wstring_view MainMenu::getName() { return ScreenName(); }

void MainMenu::Init(const std::any& args)
{
    sfxOpt =
        (std::string*)&LocalStorage::Get(StringRes::Get(StrRes::SfxToggle));

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

void MainMenu::Mount(const std::any& args)
{
    Palette levelPalette(RESOURCE_PATH MAP_PATH "forest/forest.hex");
    Palette faunaPalette(RESOURCE_PATH CHARACTER_PATH "fauna.hex");
    for (int i = 0; i < 6; i++) {
        levelPalette[i] = faunaPalette[i];
    }
    ChangeColorPalette(levelPalette);
    bgMusic.Open(RESOURCE_PATH BGM_PATH "menu.mp3");
    hoverSfx.Open(RESOURCE_PATH SFX_PATH "select.wav");
    if (LocalStorage::Get(StringRes::Get(StrRes::SfxToggle)) ==
        StringRes::Get(StrRes::OnOpt)) {
        bgMusic.Play(true, true);
    }
}

AbstractScreen* MainMenu::Clone() const { return new MainMenu; }

AbstractNavigation::NavigationRes MainMenu::Update(
    float deltaTime, const AbstractNavigation* navigation
)
{
    AbstractNavigation::NavigationRes res = navigation->NoChange();
    menu.Update(
        deltaTime,
        [&](uint8_t selected) noexcept {
            if (*sfxOpt == StringRes::Get(StrRes::OnOpt)) {
                hoverSfx.Play();
            }
        },
        [&](uint8_t selection) noexcept {
            if (*sfxOpt == StringRes::Get(StrRes::OnOpt)) {
                PlayAndForget(RESOURCE_PATH SFX_PATH "select.wav");
            }
            switch (selection) {
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

void MainMenu::Unmount()
{
    bgMusic.Close();
    hoverSfx.Close();
}
