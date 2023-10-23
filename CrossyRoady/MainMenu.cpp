#include "MainMenu.h"

using namespace ConsoleGame;

std::wstring_view MainMenu::getName() { return std::wstring_view(); }

void MainMenu::Init(const std::any& args) {}

AbstractScreen* MainMenu::Clone() const { return nullptr; }

AbstractNavigation::NavigationRes MainMenu::Update(
    float deltaTime, const AbstractNavigation* navigation
)
{
    return AbstractNavigation::NavigationRes();
}

void MainMenu::Draw(AbstractCanvas* canvas) const {}
