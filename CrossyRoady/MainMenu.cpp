#include "MainMenu.h"

#include "StringRes.h"

using namespace ConsoleGame;
using namespace std::literals;

MainMenu::MainMenu()
{
    for (int i = 0, tmp = startPos.y; i < buttons.size();
         i++, tmp += buttDim.height + gap) {
        buttons[i] = Button(
            {
                .size = buttDim,
                .pos = {startPos.x, tmp},
                .cornerSize = 5,
                .hasBorder = true,
                .background = (Color)14,
                .border = (Color)13
        },
            StringRes::Get(StrRes::SinglePlayer),
            (Color)13,
            0
        );
    }
    buttons[1].ChangeText(StringRes::Get(StrRes::TwoPlayer));
    buttons[2].ChangeText(StringRes::Get(StrRes::Progress));
    buttons[3].ChangeText(StringRes::Get(StrRes::Setting));
    buttons[4].ChangeText(StringRes::Get(StrRes::Exit));
}

const std::wstring_view MainMenu::ScreenName() { return L"Main menu"; }

std::wstring_view MainMenu::getName() { return ScreenName(); }

// spritegen2.exe ani -m ani -fps 5 -c .\bae.hex -o baeR.anisprite
void MainMenu::Init(const std::any& args)
{
    Palette levelPalette(RESOURCE_PATH MAP_PATH "forest/forest.hex");
    ChangeColorPalette(levelPalette);
}

AbstractScreen* MainMenu::Clone() const { return new MainMenu; }

AbstractNavigation::NavigationRes MainMenu::Update(
    float deltaTime, const AbstractNavigation* navigation
)
{
    timePassed += deltaTime;
    if (IsKeyMeanDown()) {
        if (timePassed > buttonDelay || lastIsUp) {
            lastIsUp = false;
            timePassed = 0;
            selected = (selected + 1 + buttons.size()) % buttons.size();
        }
    }
    if (IsKeyMeanUp()) {
        if (timePassed > buttonDelay || !lastIsUp) {
            lastIsUp = true;
            timePassed = 0;
            selected = (selected - 1 + buttons.size()) % buttons.size();
        }
    }

    auto mousePos = GetMousePos();

    for (int i = 0; i < buttons.size(); i++) {
        if (buttons[i].IsHover(mousePos)) {
            selected = i;
        }
    }

    if (IsKeyMeanSelect() || IsKeyDown(VK_LBUTTON)) {
        switch (selected) {
            case 4:
                return navigation->Exit();
        }
    }

    for (int i = 0; i < buttons.size(); i++) {
        if (selected == i) {
            buttons[i].ChangeColor((Color)14, (Color)15, (Color)13);
        } else {
            buttons[i].ChangeColor((Color)13, (Color)14, (Color)13);
        }
    }

    return navigation->NoChange();
}

void MainMenu::Draw(AbstractCanvas* canvas) const
{
    canvas->Clear((Color)6);
    Font::DrawString(canvas, "Crossy Roady", {10, 10}, 3, 1, (Color)13);
    for (const auto& butt : buttons) {
        butt.Draw(canvas);
    }
}
