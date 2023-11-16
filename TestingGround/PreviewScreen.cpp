#include "PreviewScreen.h"

using namespace std::literals;
using namespace ConsoleGame;

const std::wstring_view PreviewScreen::ScreenName()
{
    return L"PreviewScreen"sv;
}

std::wstring_view PreviewScreen::getName()
{
    return ScreenName();
}

void PreviewScreen::Init(const std::any& args)
{
    preview.Load(fileName);
}

AbstractScreen* PreviewScreen::Clone() const
{
    return new PreviewScreen(*this);
}

AbstractNavigation::NavigationRes PreviewScreen::Update(float deltaTime, const AbstractNavigation* navigation)
{
    timePassed += deltaTime;
    if (timePassed > 1) {
        auto tmp = std::filesystem::last_write_time(fileName);
        if (tmp != lastWriteTime) {
            preview.Load(fileName);
            Vec2 dim = preview.GetDim();
            drawCoord = Vec2{ .x = (_CanvasSize.x - dim.x) / 2, .y = (_CanvasSize.y - dim.y) / 2 };
            lastWriteTime = tmp;
        }
        timePassed = 0;
    }
    return navigation->NoChange();
}

void PreviewScreen::Draw(AbstractCanvas* canvas) const
{
    preview.Draw(canvas, drawCoord);
}

