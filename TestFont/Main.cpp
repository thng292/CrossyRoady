
#include "ConsoleGame.h"
#include<format>

using namespace ConsoleGame;



class TestFont : public AbstractScreen {
    Vec2 coord = {10,10};
    Vec2 CoordMouse = {10, 10};
   public:
    static const std::wstring_view ScreenName() { return L"TestFont"; }

    std::wstring_view getName() override { return ScreenName(); }

    void Init(const std::any& args) override {}

    AbstractScreen* Clone() const override { return new TestFont; }

    AbstractNavigation::NavigationRes Update(
        float deltaTime, const AbstractNavigation* navigation
    ) override
    {

        constexpr int speed = 300;
        if (IsKeyDown('W')) {
            coord.y -= speed * deltaTime;
        }
        if (IsKeyDown('S')) {
            coord.y += speed * deltaTime;
        }
        if (IsKeyDown('A')) {
            coord.x -= speed * deltaTime;
        }
        if (IsKeyDown('D')) {
            coord.x += speed * deltaTime;
        }
        if (IsKeyDown(VK_LBUTTON)) {
            CoordMouse = GetMousePos();
        }
        return navigation->NoChange();
    }

    void Draw(AbstractCanvas* canvas) const override
    {
        Font::DrawString(canvas, std::format("{} {}!", CoordMouse.x, CoordMouse.y), CoordMouse, 1, 1, Color::RED);
    }
};

auto main() -> int
{
    // Font::Load("../CrossyRoady/test.font");
    Font::Load("../CrossyRoady/fontsm.font", 1);
    auto game = std::make_unique<Game>(L"Test", GetDisplayRefreshRate());
    game->AddScreen(std::make_unique<TestFont>());
    game->Run(TestFont::ScreenName());
    return 0;
}