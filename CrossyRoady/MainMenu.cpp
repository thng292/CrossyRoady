#include "MainMenu.h"

#include "Setting.h"
#include "Progress.h"
#include "StringRes.h"

using namespace ConsoleGame;
using namespace std::literals;
using enum AbstractNavigation::NavigationAction;

const std::wstring_view MainMenu::ScreenName() { return L"Main menu"; }

std::wstring_view MainMenu::getName() { return ScreenName(); }

void MainMenu::Init(const std::any& args)
{
    bg.Init();
    sfxOpt = (std::string*)&LocalStorage::Get(R.Config.SfxToggle);

    menu.Init(
        startPos,
        buttDim,
        std::array<const std::string_view, 5>{
            R.SinglePlayer.Title,
            R.MultiPlayer.Title,
            R.Progress.Title,
            R.Setting.Title,
            R.MainMenu.Exit}
    );
    passArgs = {
        .bgMusic = &bgMusic,
        .hoverSfx = &hoverSfx,
        .menuBg = &bg,
    };
}

void MainMenu::Mount(const std::any& args)
{
    Palette levelPalette(RESOURCE_PATH MAP_PATH "forest/forest.hex");
    Palette faunaPalette(RESOURCE_PATH CHARACTER_PATH "fauna.hex");
    for (int i = 0; i < 6; i++) {
        levelPalette[i] = faunaPalette[i];
    }
    ChangeColorPalette(levelPalette);
    if (shouldCloseSharedResource) {
        bg.Mount();
        bgMusic.Open(RESOURCE_PATH BGM_PATH "menu.mp3");
        hoverSfx.Open(RESOURCE_PATH SFX_PATH "select.wav");
        if (LocalStorage::Get(R.Config.MusicToggle) == R.Config.OnOpt) {
            bgMusic.Play(true, true);
        }
    }
    menu.Mount();
}

AbstractScreen* MainMenu::Clone() const { return new MainMenu; }

AbstractNavigation::NavigationRes MainMenu::Update(
    float deltaTime, const AbstractNavigation* navigation
)
{
    bg.Update(deltaTime);
    AbstractNavigation::NavigationRes res = navigation->NoChange();
    menu.Update(
        deltaTime,
        [&](uint8_t hover) noexcept {
            if (*sfxOpt == R.Config.OnOpt) {
                hoverSfx.Play();
            }
        },
        [&](uint8_t selection) noexcept {
            if (*sfxOpt == R.Config.OnOpt) {
                PlayAndForget(RESOURCE_PATH SFX_PATH "select.wav");
            }
            switch (selection) {
                case 2:
                    shouldCloseSharedResource = false;
                    res =
                        navigation->Navigate(Progress::ScreenName(), passArgs);
                    break;
                case 3:
                    shouldCloseSharedResource = false;
                    res = navigation->Navigate(Setting::ScreenName(), passArgs);
                    break;
                case 4:
                    shouldCloseSharedResource = true;
                    res = navigation->Exit();
                    break;
            }
        }
    );
    return res;
}

void MainMenu::Draw(AbstractCanvas* canvas) const
{
    bg.Draw(canvas);
    Font::DrawString(canvas, "Crossy Roady", {10, 10}, 3, 1, (Color)13);
    menu.Draw(canvas);
}

void MainMenu::Unmount()
{
    if (shouldCloseSharedResource) {
        bg.Unmount();
        bgMusic.Close();
        hoverSfx.Close();
    }
}
