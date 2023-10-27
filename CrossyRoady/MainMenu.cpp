#include "MainMenu.h"

using namespace ConsoleGame;

using namespace std::literals;

MainMenu::MainMenu()
    : test(
          SurfaceArgs{
              .size = Vec2{100, 20},
              .pos = Vec2{50,  50},
              .cornerSize = 3,
              .hasBorder = true,
              .background = Color::LIGHT_BLUE,
              .border = Color::BLACK
},
          "Single Player"sv,
          Color::BLACK,
          1
      )
{
}

const std::wstring_view MainMenu::ScreenName() { return L"Main menu"; }

std::wstring_view MainMenu::getName() { return ScreenName(); }

void MainMenu::Init(const std::any& args)
{
    // Palette pal("another.hex");
    // ChangeColorPalette(pal);
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
    if (test.IsHover(GetMousePos())) {
        Font::DrawString(canvas, "Hello World", {0, 0});
    }
    test.Draw(canvas);
}
