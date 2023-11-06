#include "Credit.h"

#include <format>
using namespace ConsoleGame;

const std::wstring_view Credit::ScreenName() { return L"Credit"; }

std::wstring_view Credit::getName() { return ScreenName(); }

void Credit::Init(const std::any& args)
{
    buttonTitle = Button(
        {
            .size = {120,             18},
            .pos = {(384 - 120) / 2, 20},
            .cornerSize = 4,
            .hasBorder = true,
            .background = (ConsoleGame::Color::LIGHT_YELLOW),
            .border = (ConsoleGame::Color::LIGHT_MAGENTA)
    },
        "Credits",
        (ConsoleGame::Color::BLACK),
        1,
        1
    );
    buttonBack = Button(
        {
            .size = {50,             15 },
            .pos = {(384 - 50) / 2, 190},
            .cornerSize = 3,
            .hasBorder = true,
            .background = (ConsoleGame::Color::BRIGHT_WHITE),
            .border = (ConsoleGame::Color::BLACK)
    },
        "Back",
        (ConsoleGame::Color::BLACK),
        0
    );
    buttonSurface = Button(
        {
            .size = {350,             130},
            .pos = {(384 - 350) / 2, 50 },
            .cornerSize = 7,
            .hasBorder = true,
            .background = (ConsoleGame::Color::CYAN),
            .border = (ConsoleGame::Color::BLACK)
    },
        "",
        (ConsoleGame::Color::BLACK),
        0
    );
}

ConsoleGame::AbstractScreen* Credit::Clone() const { return new Credit; }

ConsoleGame::AbstractNavigation::NavigationRes Credit::Update(
    float deltaTime, const ConsoleGame::AbstractNavigation* navigation
)
{
    return navigation->NoChange();
}

void Credit::Draw(ConsoleGame::AbstractCanvas* canvas) const
{
    canvas->Clear(Color::LIGHT_CYAN);
    buttonSurface.Draw(canvas);
    Font::DrawString(
        canvas,
        std::format("{} {:>23}", "Developer", "Special Thanks"),
        {60, 65},
        1,
        1,
        Color::LIGHT_YELLOW
    );
    Font::DrawString(
        canvas,
        std::format(
            "{} {:>19} {:>22}",
            "Leader",
            "Nguyen Quang Thong",
            "Truong Toan Thinh"
        ),
        {54, 90},
        1,
        0,
        Color::BRIGHT_WHITE
    );
    Font::DrawString(
        canvas,
        std::format("{} {:>14}", "Programmer", "Vo Nhat Phuoc"),
        {30, 110},
        1,
        0,
        Color::BRIGHT_WHITE
    );
    Font::DrawString(
        canvas,
        std::format("{} {:>16} ", "Programmer", "Nguyen The Quan"),
        {30, 130},
        1,
        0,
        Color::BRIGHT_WHITE
    );
    Font::DrawString(
        canvas,
        std::format("{} {:>14}", "Artist", "Nguyen Anh Vu"),
        {53, 150},
        1,
        0,
        Color::BRIGHT_WHITE
    );
    buttonTitle.Draw(canvas);
    buttonBack.Draw(canvas);
}
