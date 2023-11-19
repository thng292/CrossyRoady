#include "Pause.h"

using namespace ConsoleGame;

const std::wstring_view Pause::ScreenName() { return L"Pause"; }

std::wstring_view Pause::getName() { return ScreenName(); }

void Pause::Init(const std::any& args) {}

AbstractScreen* Pause::Clone() const { return new Pause; }

AbstractNavigation::NavigationRes Pause::Update(
    float deltaTime, const AbstractNavigation* navigation
)
{
    if (IsKeyMeanBack()) {
        return navigation->Back();
    }
    return navigation->NoChange();
}

void Pause::Draw(AbstractCanvas* canvas) const {}
