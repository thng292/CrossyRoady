#include "Control.h"

#include <format>
using namespace ConsoleGame;

const std::wstring_view Control::ScreenName() { return L"Control"; }

std::wstring_view Control::getName() { return ScreenName(); }

void Control::Init(const std::any& args)
{
    //ListButton1.tertiaryColor = (Color)6;
    //ListButton1.primaryColor = (Color)13;
    //ListButton2.tertiaryColor = (Color)6;
    //ListButton2.primaryColor = (Color)13;
    //ListFeature.primaryColor = (Color)13;
    ListButton1.Init(
        {132, 58},
        {50, 18},
        std::array<const std::string_view, 6>{"W", "S", "A", "D", "F", "Esc"}
    );
    ListButton2.Init(
        {187, 58},
        {50, 18},
        std::array<const std::string_view, 6>{
            "Up", "Down", "Left", "Right", "Enter", ""}
    );
    ListFeature.Init(
        {245, 58},
        {120, 18},
        std::array<const std::string_view, 6>{
            "Move up",
            "Move down",
            "Move left",
            "Move right",
            "Select/use skills",
            "Pause game"}
    );
}

ConsoleGame::AbstractScreen* Control::Clone() const { return new Control; }

ConsoleGame::AbstractNavigation::NavigationRes Control::Update(
    float deltaTime, const ConsoleGame::AbstractNavigation* navigation
)
{
    return navigation->NoChange();
}

void Control::Draw(ConsoleGame::AbstractCanvas* canvas) const
{
    ListButton1.Draw(canvas);
    ListButton2.Draw(canvas);
    ListFeature.Draw(canvas);
}