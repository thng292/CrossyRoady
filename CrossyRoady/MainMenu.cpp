#include "MainMenu.h"

#include "AskSave.h"
#include "CharaSelectScreen.h"
#include "CharactersInfo.h"
#include "Progress.h"
#include "Setting.h"
#include "StringRes.h"

using namespace ConsoleGame;
using namespace std::literals;
using enum AbstractNavigation::NavigationAction;

const std::wstring_view MainMenu::ScreenName() { return L"Main menu"; }

std::wstring_view MainMenu::getName() { return ScreenName(); }

void MainMenu::Init(const std::any& args)
{
    bg.Init();
    menu.Init(
        startPos,
        buttDim,
        std::array<const std::string_view, 6>{
            R.String.SinglePlayer.Title,
            R.String.MultiPlayer.Title,
            R.String.Progress.Title,
            R.String.CharInfo.Title,
            R.String.Setting.Title,
            R.String.MainMenu.Exit}
    );
}

void MainMenu::Mount(const std::any& args)
{
    Palette levelPalette(RESOURCE_PATH MAP_PATH "space/space.hex");
    ChangeColorPalette(levelPalette);
    if (bg.IsUnmounted()) {
        bg.Mount();
    }
    audio.SwitchMusic(BGMusic::Menu);
    audio.PlayMusic();
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
        [&](uint8_t hover) noexcept { audio.PlayHoverSfx(); },
        [&](uint8_t selection) noexcept {
            audio.PlayClickSfx();
            switch (selection) {
                case 0:
                    if (std::filesystem::exists(std::filesystem::path(SAVE_PATH)
                        )) {
                        res = navigation->Navigate(AskSave::ScreenName());
                    } else {
                        res = navigation->Navigate(
                            CharacterSelectScreen::ScreenName()
                        );
                    }
                    break;
                case 2:
                    res = navigation->Navigate(Progress::ScreenName(), &bg);
                    break;
                case 3:
                    res = navigation->Navigate(CharactersInfo::ScreenName());
                    break;
                case 4:
                    res = navigation->Navigate(Setting::ScreenName(), &bg);
                    break;
                case 5:
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
    if (!bg.IsUnmounted()) {
        bg.Unmount();
    }
}
