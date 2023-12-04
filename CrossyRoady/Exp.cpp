#include "Exp.h"
using namespace ConsoleGame;

const std::wstring_view Exp::ScreenName() { return L"Exp"; }

std::wstring_view Exp::getName() { return ScreenName(); }

void Exp::Init(const std::any& args) {}

ConsoleGame::AbstractScreen* Exp::Clone() const { return new Exp; }

ConsoleGame::AbstractNavigation::NavigationRes Exp::Update(
    float deltaTime, const ConsoleGame::AbstractNavigation* navigation
)
{
    return navigation->NoChange();
}

void Exp::Draw(ConsoleGame::AbstractCanvas* canvas) const
{
    Font::DrawString(
        canvas,
        "When playing in a game, you'll receive",
        {137, 60},
        1,
        0,
        (Color)15
    );
    Font::DrawString(
        canvas, "EXP to level-up in Progress.", {137, 75}, 1, 0, (Color)15
    );
    Font::DrawString(
        canvas,
        "800 exp points -> Level-up in Progress",
        {137, 90},
        1,
        0,
        (Color)15
    );
    Font::DrawString(
        canvas,
        "In each map, if you get enough points",
        {137, 115},
        1,
        0,
        (Color)15
    );
    Font::DrawString(
        canvas,
        "in one play, you will receive a new",
        {137, 130},
        1,
        0,
        (Color)15
    );
    Font::DrawString(canvas, "character.", {137, 145}, 1, 0, (Color)15);
    Font::DrawString(canvas, "Easy: 400", {160, 165}, 1, 0, (Color)15);
    Font::DrawString(canvas, "Hard: 100", {280, 165}, 1, 0, (Color)15);
    Font::DrawString(canvas, "Normal: 300", {160, 180}, 1, 0, (Color)15);
    Font::DrawString(canvas, "Auto: 200", {280, 180}, 1, 0, (Color)15);
}
