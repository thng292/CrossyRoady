#include "Result.h"

using namespace ConsoleGame;

const std::wstring_view Result::ScreenName() { return L"Result"; }

std::wstring_view Result::getName() { return std::wstring_view(); }

void Result::Init(const std::any& args)
{
    // score
    // skill use
    // time played
    // difficulty
    // num of mob collided
    // mode
    // map type
    // chara type
}

AbstractScreen* Result::Clone() const { return nullptr; }

AbstractNavigation::NavigationRes Result::Update(
    float deltaTime, const AbstractNavigation* navigation
)
{
    return AbstractNavigation::NavigationRes();
}

void Result::Draw(AbstractCanvas* canvas) const {}
