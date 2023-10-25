#include "MainMenu.h"

using namespace ConsoleGame;

const std::wstring_view MainMenu::ScreenName() { return L"Main menu"; }

std::wstring_view MainMenu::getName() { return ScreenName(); }

void MainMenu::Init(const std::any& args)
{
    Palette pal("another.hex");
    ChangeColorPalette(pal);
}

AbstractScreen* MainMenu::Clone() const { return new MainMenu; }

AbstractNavigation::NavigationRes MainMenu::Update(
    float deltaTime, const AbstractNavigation* navigation
)
{
    return navigation->NoChange();
}

void MainMenu::Draw(AbstractCanvas* canvas) const
{
    auto dd = Font::GetDim(0);
    for (int i = 0; i < 16; i++) {
        Font::DrawString(canvas, "Hello World", {0, i * dd.height}, 1, 1, (Color)i);
    }
}
