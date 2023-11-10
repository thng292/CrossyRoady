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
        canvas, "When plaing in a game, you'll receive", {137, 60}, 1, 0
    );
    Font::DrawString(
        canvas, "EXP to level up after each play.", {137, 75}, 1, 0
    );
    Font::DrawString(
        canvas, "After each level, you can unlock new", {137, 90}, 1, 0
    );
    Font::DrawString(
        canvas, "characters with unique and interesting", {137, 105}, 1, 0
    );
    Font::DrawString(
        canvas, "skills, along with new game maps with", {137, 120}, 1, 0
    );
    Font::DrawString(
        canvas, "special effects, or you can choose to", {137, 135}, 1, 0
    );
    Font::DrawString(
        canvas, "upgrade your current character.", {137, 150}, 1, 0
    );
}