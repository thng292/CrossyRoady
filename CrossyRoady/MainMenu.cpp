#include "MainMenu.h"

using namespace ConsoleGame;

const std::wstring_view MainMenu::ScreenName() { return L"Main menu"; }

std::wstring_view MainMenu::getName() { return ScreenName(); }

void MainMenu::Init(const std::any& args) {}

AbstractScreen* MainMenu::Clone() const { return new MainMenu; }

AbstractNavigation::NavigationRes MainMenu::Update(
    float deltaTime, const AbstractNavigation* navigation
)
{
    return navigation->NoChange();
}

void MainMenu::Draw(AbstractCanvas* canvas) const {}
