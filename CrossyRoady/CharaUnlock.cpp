#include "CharaUnlock.h"

#include <format>

using namespace ConsoleGame;
using namespace GameType;

constexpr Color bgColor = Color(0);
constexpr Color fontColor = Color(1);
constexpr int XCoord = 140;

const std::string_view basePath = RESOURCE_PATH EXTRA_PATH;

void CharaUnlock::LoadStuff()
{
    auto spritePath = std::format("{}{}-por.sprite", basePath, charaName);
    auto palettePath = std::format("{}{}-por.hex", basePath, charaName);
    ChangeColorPalette(Palette(palettePath));
    portrait.Load(spritePath);
    auto tmp = portrait.GetDim();
    portraitPos = {
        (XCoord - tmp.width) / 2, (_CanvasSize.height - tmp.height) / 2};
}

const std::wstring_view CharaUnlock::ScreenName() { return L"CharaUnlock"; }

std::wstring_view CharaUnlock::getName() { return ScreenName(); }

void CharaUnlock::Init(const std::any& args)
{
    if (args.has_value()) {
        mapType = std::any_cast<MapType>(args);
    }
    menu.primaryColor = (Color)1;
    menu.secondaryColor = (Color)2;
    menu.tertiaryColor = (Color)0;
    charaName = fileCharName[mapType];
    switch (mapType) {
        case FOREST:
            charStuff = R.String.Character.Fauna;
            break;
        case CITY:
            charStuff = R.String.Character.Irys;
            break;
        case HOUSE:
            charStuff = R.String.Character.Mumei;
            break;
        case DESERT:
            charStuff = R.String.Character.Kronii;
            break;
        case SPACE:
            charStuff = R.String.Character.Sana;
            break;
        case CASINO:
            charStuff = R.String.Character.Bae;
            break;
    }
    menu.Init({295, 160 + yShift}, {80, 18}, {R.String.Next});
    LoadStuff();
}

AbstractScreen* CharaUnlock::Clone() const { return new CharaUnlock; }

AbstractNavigation::NavigationRes CharaUnlock::Update(
    float deltaTime, const AbstractNavigation* navigation
)
{
    auto res = navigation->NoChange();
    menu.Update(
        deltaTime,
        [&](uint8_t) noexcept { audio.PlayHoverSfx(); },
        [&](uint8_t selection) noexcept {
            switch (selection) {
                case 0:
                    res =
                        navigation->Navigate(CharacterSelectScreen::ScreenName()
                        );
                    break;
            }
        }
    );
    return res;
}

void CharaUnlock::Draw(AbstractCanvas* canvas) const
{
    canvas->Clear(bgColor);
    portrait.Draw(canvas, portraitPos);

    Font::DrawString(
        canvas, R.String.CharUnlock.Unlock, {XCoord - 55, 10}, 1, 1, fontColor
    );

    Font::DrawString(
        canvas, charStuff.Name, {XCoord - 12, 10 + yShift}, 2, 1, fontColor
    );
    Font::DrawStringInBox(
        canvas,
        charStuff.Desc,
        {{XCoord, 50 + yShift}, {_CanvasSize.width - XCoord - 20, 100}},
        1,
        0,
        fontColor
    );
    Font::DrawString(
        canvas, R.String.CharInfo.Skill, {XCoord, 110 + yShift}, 1, 0, fontColor
    );
    Font::DrawStringInBox(
        canvas,
        charStuff.Skill,
        {{XCoord, 110 + Font::GetDim(0).height + 5 + yShift},
         {_CanvasSize.width - XCoord - 20, 30}},
        1,
        0,
        fontColor
    );
    menu.Draw(canvas);
}
