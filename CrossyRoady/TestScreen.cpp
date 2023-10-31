#include "TestScreen.h"

#include "MainMenu.h"
using namespace ConsoleGame;

const std::wstring_view TestScreen::ScreenName() { return L"ScreenName"; }

std::wstring_view TestScreen::getName() { return ScreenName(); }

void TestScreen::Init(const std::any& args)
{
    Mob mob;
    mob.setSize(size);
    road.setY(poss.y);
    road.setMob(mob);
    road.setHeight(32);
    road.setDirec(1);
    road.Init();
}

AbstractScreen* TestScreen::Clone() const { return new TestScreen; }

AbstractNavigation::NavigationRes TestScreen::Update(
    float deltaTime, const AbstractNavigation* navigation
)
{
    road.updateCoord();
    return navigation->NoChange();
}

void TestScreen::Draw(AbstractCanvas* canvas) const
{
    Color color = Color::BLACK;
    auto listMob = road.getListMob();
    for (auto pos : listMob) {
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                if (pos > 0) (*canvas)[road.getY() + i][pos + j] = color;
            }
        }
    }
}
