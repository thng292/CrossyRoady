#include "CharaSelectScreen.h"

#include "StringRes.h"

using namespace ConsoleGame;

const std::wstring_view CharacterSelectScreen::ScreenName()
{
    return L"CharSelect";
}

std::wstring_view CharacterSelectScreen::getName() { return ScreenName(); }

void CharacterSelectScreen::Init(const std::any& args)
{
    try {
        unlocked = std::stoi(LocalStorage::Get(R.Config.CharUnlocked));
    } catch (...) {
        unlocked = 1;
    }

    
}

void CharacterSelectScreen::Mount(const std::any& args)
{
    keyCounter = 0;
    mouseCounter = 0;
}

AbstractScreen* CharacterSelectScreen::Clone() const
{
    return new CharacterSelectScreen;
}

AbstractNavigation::NavigationRes CharacterSelectScreen::Update(
    float deltaTime, const AbstractNavigation* navigation
)
{
    return AbstractNavigation::NavigationRes();
}

void CharacterSelectScreen::Draw(AbstractCanvas* canvas) const {}

void CharacterSelectScreen::Unmount()
{
    for (auto& sprite : charAvaMenu) {
        sprite.Unload();
    }
}
