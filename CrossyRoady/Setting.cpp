#include "Setting.h"

#include "Common.h"
#include "StringRes.h"

using namespace ConsoleGame;

const std::wstring_view Setting::ScreenName() { return L"SettingScreen"; }

std::wstring_view Setting::getName() { return ScreenName(); }

void Setting::UpdateMusicTitle()
{
    MusicTitle.clear();
    MusicTitle += R.Setting.Music;
    MusicTitle += ": ";
    if (LocalStorage::Get(R.Setting.Music) == R.Config.OnOpt) {
        MusicTitle += R.Config.OnOpt;
    } else {
        MusicTitle += R.Config.OffOpt;
    }
    menu.buttons[0].ChangeText(MusicTitle);
}

void Setting::UpdateSfxTitle()
{
    SfxTitle.clear();
    SfxTitle += R.Setting.Sfx;
    SfxTitle += ": ";
    if (LocalStorage::Get(R.Setting.Sfx) == R.Config.OnOpt) {
        SfxTitle += R.Config.OnOpt;
    } else {
        SfxTitle += R.Config.OffOpt;
    }
    menu.buttons[1].ChangeText(SfxTitle);
}

void Setting::Init(const std::any& args)
{
    auto tmp = std::any_cast<SharedMenuStuff>(args);
    bg = tmp.menuBg;
    hoverSfx = tmp.hoverSfx;
    bgMusic = tmp.bgMusic;
    UpdateMusicTitle();
    UpdateSfxTitle();

    sfxOpt = (std::string*)&LocalStorage::Get(R.Config.SfxToggle);

    title = Button(
        {
            .size = {150,        24},
            .pos = {startPos.x, 30},
            .cornerSize = 5,
            .hasBorder = true,
            .background = (Color)14,
            .border = (Color)13
    },
        R.Setting.Title,
        (Color)13,
        1
    );
    menu.Init(
        startPos,
        buttDim,
        std::array<const std::string_view, 5>{
            MusicTitle,
            SfxTitle,
            R.HowToPlay.Title,
            R.Credit.Title,
            R.Back}
    );
}

AbstractScreen* Setting::Clone() const { return new Setting; }

void Setting::Mount(const std::any& args) { menu.Mount(); }

AbstractNavigation::NavigationRes Setting::Update(
    float deltaTime, const AbstractNavigation* navigation
)
{
    AbstractNavigation::NavigationRes res = navigation->NoChange();
    bg->Update(deltaTime);
    menu.Update(
        deltaTime,
        [&](uint8_t hover) noexcept {
            if (*sfxOpt == R.Config.OnOpt) {
                hoverSfx->Play();
            }
        },
        [&](uint8_t selection) noexcept {
            if (*sfxOpt == R.Config.OnOpt) {
                PlayAndForget(RESOURCE_PATH SFX_PATH "select.wav");
            }

            switch (selection) {
                case 0:
                    if (LocalStorage::Get(R.Config.MusicToggle) ==
                        R.Config.OnOpt) {
                        LocalStorage::Set(
                            R.Config.MusicToggle, R.Config.OffOpt
                        );
                        bgMusic->Pause();
                    } else {
                        LocalStorage::Set(
                            R.Config.MusicToggle, R.Config.OnOpt
                        );
                        bgMusic->Resume();
                    }
                    UpdateMusicTitle();
                    break;
                case 1:
                    if (*sfxOpt == R.Config.OnOpt) {
                        LocalStorage::Set(
                            R.Config.SfxToggle, R.Config.OffOpt
                        );
                    } else {
                        LocalStorage::Set(
                            R.Config.SfxToggle, R.Config.OnOpt
                        );
                    }
                    UpdateSfxTitle();
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
