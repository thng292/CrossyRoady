#include "MainMenu.h"

#include "StringRes.h"

using namespace ConsoleGame;

using namespace std::literals;

MainMenu::MainMenu()
    : test(
          {
              .size = Vec2{100, 20},
              .pos = Vec2{50,  50},
              .cornerSize = 3,
              .hasBorder = true,
              .background = Color::LIGHT_BLUE,
              .border = Color::BLACK
},
          StringRes::Get(StrRes::SinglePlayer),
          Color::BLACK,
          0
      )
{
}

const std::wstring_view MainMenu::ScreenName() { return L"Main menu"; }

std::wstring_view MainMenu::getName() { return ScreenName(); }

void MainMenu::Init(const std::any& args)
{
    prep.Load("resource/character/sana.hex");
    ChangeColorPalette(prep);
    pre.Load("resource/character/back-1.sprite");
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
    /*if (test.IsHover(GetMousePos())) {
        Font::DrawString(canvas, "Hello World", {0, 0});
    }
    test.Draw(canvas);*/
    pre.Paint(canvas, {0, 0});
    for (char i = 0; i < 16; i++) {
        Font::DrawString(canvas, "Test", {100, i * 9}, 1, 0, (Color)i);
    }
}
