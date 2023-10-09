#include "DevScreen.h"

using namespace std::literals;
using namespace ConsoleGame;

std::wstring_view DevScreen::getName() { return L"DevScreen"sv; }

void DevScreen::Init(const std::any& args) {}

AbstractScreen* DevScreen::Clone() const { return new DevScreen; }

AbstractNavigation::NavigationRes DevScreen::Update(
    const InputRecords& inputs,
    float deltaTime,
    const AbstractNavigation* navigation
)
{
    move = (move + 1) % 30;
    return navigation->NoChange();
}

void DevScreen::Draw(AbstractCanvas* canvas) const
{
    constexpr Vec2 upLeft{.x = 15, .y = 30};
    constexpr Vec2 dim{.width = 30, .height = 30};

    for (int i = 0; i < dim.width; i++) {
        for (int j = 0; j < dim.height; j++) {
            (*canvas)[i + upLeft.x + move / 2][j + upLeft.y + move / 2] = Color::YELLOW;
        }
    }
}

void DevScreen::DeInit() {}
