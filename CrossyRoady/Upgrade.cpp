#include "Upgrade.h"

using namespace ConsoleGame;

const std::wstring_view Upgrade::ScreenName() { return L"Upgrade"; }

std::wstring_view Upgrade::getName() { return ScreenName(); }

void Upgrade::Init(const std::any& args) {}

ConsoleGame::AbstractScreen* Upgrade::Clone() const { return new Upgrade; }

ConsoleGame::AbstractNavigation::NavigationRes Upgrade::Update(
    float deltaTime, const ConsoleGame::AbstractNavigation* navigation
)
{
    return navigation->NoChange();
}

void Upgrade::Draw(ConsoleGame::AbstractCanvas* canvas) const
{
    Font::DrawString(
        canvas,
        "The characters you have unlocked can",
        {137, 60},
        1,
        0,
        (Color)15
    );
    Font::DrawString(canvas, "be upgraded.", {137, 75}, 1, 0, (Color)15);
    Font::DrawString(
        canvas,
        "Each upgrade will help the character",
        {137, 100},
        1,
        0,
        (Color)15
    );
    Font::DrawString(
        canvas,
        "reduce skill cooldown time.",
        {137, 115},
        1,
        0,
        (Color)15
    );
    Font::DrawString(canvas, "Menu", {137, 140}, 1, 0, (Color)15);
    Font::DrawString(canvas, " ->", {160, 140}, 1, 0, (Color)15);
    Font::DrawString(canvas, " Characters", {178, 140}, 1, 0, (Color)15);
    Font::DrawString(canvas, "to upgrade your", {250, 140}, 1, 0, (Color)15);
    Font::DrawString(canvas, "characters.", {137, 155}, 1, 0, (Color)15);
}
