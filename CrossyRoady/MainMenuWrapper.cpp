#include "MainMenuWrapper.h"

#include "StringRes.h"

using namespace ConsoleGame;

const std::wstring_view MainMenuWrapper::ScreenName()
{
    return L"MainMenuWrapper";
}

std::wstring_view MainMenuWrapper::getName() { return ScreenName(); }

void MainMenuWrapper::Init(const std::any& args) {}

void MainMenuWrapper::Mount(const std::any& args)
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

AbstractScreen* MainMenuWrapper::Clone() const { return new MainMenuWrapper; }

AbstractNavigation::NavigationRes MainMenuWrapper::Update(
    float deltaTime, const AbstractNavigation* navigation
)
{
    return AbstractNavigation::NavigationRes();
}

void MainMenuWrapper::Draw(AbstractCanvas* canvas) const {}

void MainMenuWrapper::Unmount()
{
    hoverSfx.Close();
    bgMusic.Close();
}
