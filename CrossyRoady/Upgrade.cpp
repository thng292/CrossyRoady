#include "Upgrade.h"

using namespace ConsoleGame;

const std::wstring_view Upgrade::ScreenName() { return L"Upgrade"; }

std::wstring_view Upgrade::getName() { return ScreenName(); }

void Upgrade::Init(const std::any& args)
{
    arrow = ArrowButton(
        {
            .pos = {137, 150},
            .cornerSize = 4,
            .hasBorder = true,
            .background = Color::BRIGHT_WHITE,
            .border = Color::BLACK,
    },
        true
    );
}

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
        canvas, "The characters you have unlocked can", {137, 60}, 1, 0
    );
    Font::DrawString(canvas, "be upgraded.", {137, 75}, 1, 0);
    Font::DrawString(
        canvas, "To upgrade, you need Upgrade Points.", {137, 100}, 1, 0
    );
    Font::DrawString(
        canvas, "You will receive Upgrade Points after", {137, 115}, 1, 0
    );
    Font::DrawString(canvas, "each level-up", {137, 130}, 1, 0);
    Font::DrawString(canvas, "Menu", {137, 155}, 1, 0);
    Font::DrawString(canvas, "Characters to upgrade your", {180, 155}, 1, 0);
    Font::DrawString(canvas, "characters.", {137, 170}, 1, 0);

    arrow.Draw(canvas);
}