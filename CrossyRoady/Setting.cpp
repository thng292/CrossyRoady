#include "Setting.h"

#include "Common.h"
#include "Credit.h"
#include "HowToPlay.h"
#include "StringRes.h"
using namespace ConsoleGame;

const std::wstring_view Setting::ScreenName() { return L"SettingScreen"; }

std::wstring_view Setting::getName() { return ScreenName(); }

void Setting::UpdateMusicTitle()
{
    MusicTitle.clear();
    MusicTitle += R.String.Setting.Music;
    MusicTitle += ": ";
    if (R.Config.Music) {
        MusicTitle += R.String.Setting.OnOpt;
    } else {
        MusicTitle += R.String.Setting.OffOpt;
    }
    menu.buttons[0].ChangeText(MusicTitle);
}

void Setting::UpdateSfxTitle()
{
    SfxTitle.clear();
    SfxTitle += R.String.Setting.Sfx;
    SfxTitle += ": ";
    if (R.Config.Sfx) {
        SfxTitle += R.String.Setting.OnOpt;
    } else {
        SfxTitle += R.String.Setting.OffOpt;
    }
    menu.buttons[1].ChangeText(SfxTitle);
}

void Setting::Init(const std::any& args)
{
    bg = std::any_cast<MenuBG*>(args);
    UpdateMusicTitle();
    UpdateSfxTitle();

    title = Button(
        {
            .size = {150,        24},
            .pos = {startPos.x, 30},
            .cornerSize = 5,
            .hasBorder = true,
            .background = (Color)14,
            .border = (Color)13
    },
        R.String.Setting.Title,
        (Color)13,
        1
    );
    menu.Init(
        startPos,
        buttDim,
        std::array<const std::string_view, 5>{
            MusicTitle,
            SfxTitle,
            R.String.HowToPlay.Title,
            R.String.Credit.Title,
            R.String.Back}
    );
}

AbstractScreen* Setting::Clone() const { return new Setting; }

void Setting::Mount(const std::any& args) {}

AbstractNavigation::NavigationRes Setting::Update(
    float deltaTime, const AbstractNavigation* navigation
)
{
    AbstractNavigation::NavigationRes res = navigation->NoChange();
    bg->Update(deltaTime);
    menu.Update(
        deltaTime,
        [&](uint8_t hover) noexcept { audio.PlayHoverSfx(); },
        [&](uint8_t selection) noexcept {
            audio.PlayClickSfx();
            switch (selection) {
                case 0:
                    R.Config.Music = ~R.Config.Music;
                    audio.UpdateMusicState();
                    UpdateMusicTitle();
                    break;
                case 1:
                    R.Config.Sfx = ~R.Config.Sfx;
                    UpdateSfxTitle();
                    break;
                case 2:
                    res = navigation->Navigate(HowToPlay::ScreenName(), bg);
                    break;
                case 3:
                    res = navigation->Navigate(Credit::ScreenName(), bg);
                    break;
                case 4:
                    res = navigation->Back();
                    break;
            }
        }
    );
    return res;
}

void Setting::Draw(AbstractCanvas* canvas) const
{
    bg->Draw(canvas);
    title.Draw(canvas);
    menu.Draw(canvas);
}

void Setting::Unmount() { R.Config.Save(CONFIG_PATH); }
