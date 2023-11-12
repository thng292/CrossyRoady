#include "Credit.h"

#include <format>

#include "Common.h"
#include "StringRes.h"
using namespace ConsoleGame;

const std::wstring_view Credit::ScreenName() { return L"Credit"; }

std::wstring_view Credit::getName() { return ScreenName(); }

void Credit::Init(const std::any& args)
{
    bg = std::any_cast<MenuBG*>(args);
    buttonTitle = Button(
        {
            .size = {120,             22},
            .pos = {(384 - 120) / 2, 20},
            .cornerSize = 4,
            .hasBorder = true,
            .background = (Color)14,
            .border = (Color)13,
    },
        R.String.Credit.Title,
        ((Color)13),
        1,
        1
    );
    backButt.Init({(384 - 50) / 2, 190}, {50, 18}, {R.String.Back});
    surface = Surface({
        .size = {350,             130},
        .pos = {(384 - 350) / 2, 50 },
        .cornerSize = 7,
        .hasBorder = true,
        .background = (Color)14,
        .border = ((Color)13)
    });
}

ConsoleGame::AbstractScreen* Credit::Clone() const { return new Credit; }

ConsoleGame::AbstractNavigation::NavigationRes Credit::Update(
    float deltaTime, const ConsoleGame::AbstractNavigation* navigation
)
{
    auto res = navigation->NoChange();
    bg->Update(deltaTime);
    backButt.Update(
        deltaTime,
        [](uint8_t) noexcept {},
        [&](uint8_t) noexcept {
            audio.PlayClickSfx();
            res = navigation->Back();
        }
    );
    return res;
}

void Credit::Draw(ConsoleGame::AbstractCanvas* canvas) const
{
    bg->Draw(canvas);
    surface.Draw(canvas);
    Font::DrawString(
        canvas,
        std::format("{} {:>23}", "Developer", "Special Thanks"),
        {60, 65},
        1,
        1,
        (Color)15
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
        (Color)13
    );
    Font::DrawString(
        canvas,
        std::format("{} {:>14}", "Programmer", "Vo Nhat Phuoc"),
        {30, 110},
        1,
        0,
        (Color)13
    );
    Font::DrawString(
        canvas,
        std::format("{} {:>16} ", "Programmer", "Nguyen The Quan"),
        {30, 130},
        1,
        0,
        (Color)13
    );
    Font::DrawString(
        canvas,
        std::format("{} {:>14}", "Artist", "Nguyen Anh Vu"),
        {53, 150},
        1,
        0,
        (Color)13
    );
    buttonTitle.Draw(canvas);
    backButt.Draw(canvas);
}
