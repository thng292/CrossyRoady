#include "GameMap.h"

using namespace ConsoleGame;

const std::wstring_view GameMap::ScreenName() { return L"GameMap"; }

std::wstring_view GameMap::getName() { return ScreenName(); }

void GameMap::Init(const std::any& args) {}

AbstractScreen* GameMap::Clone() const { return new GameMap; }

AbstractNavigation::NavigationRes GameMap::Update(
    float deltaTime, const AbstractNavigation* navigation
)
{
    return navigation->NoChange();
}

void GameMap::Draw(AbstractCanvas* canvas) const {}
