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
        "When plaing in a game, you'll receive",
        {137, 60},
        1,
        0,
        (Color)15
    );
    Font::DrawString(canvas, "EXP", {137, 75}, 1, 0, (Color)6);
    Font::DrawString(canvas, "to", {161, 75}, 1, 0, (Color)15);
    Font::DrawString(canvas, "level-up", {179, 75}, 1, 0, (Color)6);
    Font::DrawString(canvas, "after each play.", {233, 75}, 1, 0, (Color)15);
    Font::DrawString(
        canvas, "After each level, you can unlock", {137, 100}, 1, 0, (Color)15
    );
    Font::DrawString(canvas, "new", {335, 100}, 1, 0, (Color)6);
    Font::DrawString(canvas, "characters", {137, 115}, 1, 0, (Color)6);
    Font::DrawString(
        canvas, "with unique and interesting", {203, 115}, 1, 0, (Color)15
    );
    Font::DrawString(canvas, "skills, along with", {137, 130}, 1, 0, (Color)15);
    Font::DrawString(canvas, "new game maps", {251, 130}, 1, 0, (Color)6);
    Font::DrawString(canvas, "with", {335, 130}, 1, 0, (Color)15);
    Font::DrawString(
        canvas,
        "special effects, or you can choose to",
        {137, 145},
        1,
        0,
        (Color)15
    );
    Font::DrawString(canvas, "upgrade", {137, 160}, 1, 0, (Color)6);
    Font::DrawString(
        canvas, "your current character.", {185, 160}, 1, 0, (Color)15
    );
}
