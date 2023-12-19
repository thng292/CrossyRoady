#include "CharaUnlock.h"

#include <format>

using namespace ConsoleGame;
using namespace GameType;

constexpr char bgColor = char(0);
constexpr char fontColor = char(1);
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
        (XCoord - tmp.width) / 2, (_CanvasSize.height - tmp.height) / 2
    };
}

const std::wstring_view CharaUnlock::ScreenName() { return L"CharaUnlock"; }

std::wstring_view CharaUnlock::getName() { return ScreenName(); }

void CharaUnlock::Init(const std::any& args)
{
    if (args.has_value()) {
        mapType = std::any_cast<MapType>(args);
    }
    menu.primaryColor = (char)1;
    menu.secondaryColor = (char)2;
    menu.tertiaryColor = (char)0;
    charaName = fileCharName[mapType];
    switch (mapType) {
        case FOREST:
            charStuff = R.String.Character.Fauna;
            R.Config.FaunaUnlocked = true;
            break;
        case CITY:
            charStuff = R.String.Character.Irys;
            R.Config.IrysUnlocked = true;
            break;
        case HOUSE:
            charStuff = R.String.Character.Mumei;
            R.Config.MumeiUnlocked = true;
            break;
        case DESERT:
            charStuff = R.String.Character.Kronii;
            R.Config.KroniiUnlocked = true;
            break;
        case SPACE:
            charStuff = R.String.Character.Sana;
            R.Config.SanaUnlocked = true;
            break;
        case CASINO:
            charStuff = R.String.Character.Bae;
            R.Config.BaeUnlocked = true;
            break;
    }
    menu.Init({295, 160 + yShift}, {80, 18}, {R.String.Next});
    unlockSfx.Open(RESOURCE_PATH SFX_PATH "unlock.wav");
    if (R.Config.Sfx) {
        unlockSfx.Play();
    }
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
                    res = navigation->PopBackTo(MainMenu::ScreenName());
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

    ConsoleGame::Font::DrawString(
        canvas, R.String.CharUnlock.Unlock, {XCoord - 55, 10}, 1, 1, fontColor
    );

    ConsoleGame::Font::DrawString(
        canvas, charStuff.Name, {XCoord - 12, 10 + yShift}, 2, 1, fontColor
    );
    ConsoleGame::Font::DrawStringInBox(
        canvas,
        charStuff.Desc,
        {{XCoord, 50 + yShift}, {_CanvasSize.width - XCoord - 20, 100}},
        1,
        0,
        fontColor
    );
    ConsoleGame::Font::DrawString(
        canvas, R.String.CharInfo.Skill, {XCoord, 110 + yShift}, 1, 0, fontColor
    );
    ConsoleGame::Font::DrawStringInBox(
        canvas,
        charStuff.Skill,
        {{XCoord, 110 + ConsoleGame::Font::GetDim(0).height + 5 + yShift},
         {_CanvasSize.width - XCoord - 20, 30}},
        1,
        0,
        fontColor
    );
    menu.Draw(canvas);
}
