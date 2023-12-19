#include "Credit.h"

#include <format>

#include "Common.h"
#include "StringRes.h"
using namespace ConsoleGame;

const std::wstring_view Credit::ScreenName() { return L"Credit"; }

std::wstring_view Credit::getName() { return ScreenName(); }

void Credit::Init(const std::any& args)
{
    if (args.has_value()) {
        bg = std::any_cast<MenuBG*>(args);
    }
    buttonTitle = Button(
        {
            .size = {120, 22},
            .pos = {(384 - 120) / 2, 20},
            .cornerSize = 4,
            .hasBorder = true,
            .background = (char)14,
            .border = (char)13,
        },
        R.String.Credit.Title,
        ((char)13),
        1,
        1
    );
    backButt.Init({(384 - 50) / 2, 190}, {50, 18}, {R.String.Back});
    surface = Surface(
        {.size = {350, 130},
         .pos = {(384 - 350) / 2, 50},
         .cornerSize = 7,
         .hasBorder = true,
         .background = (char)14,
         .border = ((char)13)}
    );
}

ConsoleGame::AbstractScreen* Credit::Clone() const { return new Credit; }

ConsoleGame::AbstractNavigation::NavigationRes Credit::Update(
    float deltaTime, const ConsoleGame::AbstractNavigation* navigation
)
{
    auto res = navigation->NoChange();
    if (bg) {
        bg->Update(deltaTime);
    }
    backButt.Update(
        deltaTime,
        [](uint8_t) noexcept {},
        [&](uint8_t) noexcept {
            audio.PlayClickSfx();
            res = navigation->Back(1);
        }
    );
    return res;
}

void Credit::Draw(ConsoleGame::AbstractCanvas* canvas) const
{
    if (bg) {
        bg->Draw(canvas);
    }
    surface.Draw(canvas);
    ConsoleGame::Font::DrawString(
        canvas,
        std::format("{} {:>23}", "Developer", "Special Thanks"),
        {60, 65},
        1,
        1,
        (char)15
    );
    ConsoleGame::Font::DrawString(
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
        (char)13
    );
    ConsoleGame::Font::DrawString(
        canvas,
        std::format(
            "{} {:>14} {:>21}", "Programmer", "Vo Nhat Phuoc", "Kebbie"
        ),
        {30, 105},
        1,
        0,
        (char)13
    );
    ConsoleGame::Font::DrawString(
        canvas,
        std::format(
            "{} {:>16} {:>19}", "Programmer", "Nguyen The Quan", "Kay Yu"
        ),
        {30, 120},
        1,
        0,
        (char)13
    );
    ConsoleGame::Font::DrawString(
        canvas,
        std::format(
            "{} {:>14} {:>23}", "Artist", "Nguyen Anh Vu", "potato7192"
        ),
        {53, 135},
        1,
        0,
        (char)13
    );
    ConsoleGame::Font::DrawString(
        canvas, std::format("{}", "Jeremy Robson"), {254, 150}, 1, 0, (char)13
    );
    buttonTitle.Draw(canvas);
    backButt.Draw(canvas);
}
