#include "MapSelect.h"

#include "StringRes.h"
using namespace ConsoleGame;
constexpr auto BGPrimary = (ConsoleGame::Color)12;
constexpr auto BGSecond = (ConsoleGame::Color)11;

const std::wstring_view MapSelect::ScreenName() { return L"MapSelect"; }

std::wstring_view MapSelect::getName() { return ScreenName(); }

void MapSelect::Init(const std::any& args)
{
    selectedMap = R.Config.MapUnlocked - 1;
    ChangeColorPalette(Palette(
        std::format(RESOURCE_PATH MAP_PATH "{}/{}.hex", fileMapName[selectedMap], fileMapName[selectedMap])
    ));
    preview.Load(std::format(
        RESOURCE_PATH MAP_PATH "{}/preview.sprite", fileMapName[selectedMap]
    ));
   /* menu.Init(
        {150, 30},
        {100, 19},
        {
            R.String.MapSelect.Play,
            R.String.MapSelect.Difficulty,
            R.String.MapSelect.Mode,
            R.String.MapSelect.Music,
            R.String.MapSelect.Debuff,
            R.String.Back,
        }
    );*/
}

AbstractScreen* MapSelect::Clone() const { return new MapSelect; }

AbstractNavigation::NavigationRes MapSelect::Update(
    float deltaTime, const AbstractNavigation* navigation
)
{
    return navigation->NoChange();
}

void MapSelect::Draw(AbstractCanvas* canvas) const
{
    canvas->Clear(BGPrimary);
    Font::DrawString(
        canvas, R.String.CharSelect.Title, {40, 10}, 1, 1, (Color)14
    );
    preview.Draw(canvas, {30, 30});
    menu.Draw(canvas);
}
