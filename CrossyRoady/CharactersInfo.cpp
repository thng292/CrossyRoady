#include "CharactersInfo.h"

#include <format>

using namespace ConsoleGame;

constexpr Color bgColor = Color(0);
constexpr Color fontColor = Color(1);
constexpr int XCoord = 140;

const std::string_view basePath = RESOURCE_PATH EXTRA_PATH;

void CharactersInfo::LoadStuff()
{
    auto spritePath =
        std::format("{}{}-por.sprite", basePath, fileCharName[currentSelect]);
    auto palettePath =
        std::format("{}{}-por.hex", basePath, fileCharName[currentSelect]);
    ChangeColorPalette(Palette(palettePath));
    portrait.Load(spritePath);
    auto tmp = portrait.GetDim();
    portraitPos = {
        (XCoord - tmp.width) / 2, (_CanvasSize.height - tmp.height) / 2};
}

CharactersInfo::CharactersInfo()
    : leftArr(
          SurfaceArgs{
              .pos = {5, 112 - 5},
              .cornerSize = 10,
              .border = fontColor,
          },
          false
      ),
      rightArr(
          SurfaceArgs{
              .pos = {369, 112 - 5},
              .cornerSize = 10,
              .border = fontColor,
          },
          true
      )
{
}

const std::wstring_view CharactersInfo::ScreenName() { return L"CharInfo"; }

std::wstring_view CharactersInfo::getName() { return ScreenName(); }

void CharactersInfo::Init(const std::any& args)
{
    menu.primaryColor = (Color)1;
    menu.secondaryColor = (Color)2;
    menu.tertiaryColor = (Color)0;
    menu.Init({295, 175}, {80, 18}, {R.String.CharInfo.Upgrade, R.String.Back});
    charStuff = (CharStuff*)&R.String.Character;
    UpgradePointStr = std::format(
        "{}{}", R.String.CharInfo.UpgradePoint, int(R.Config.UpgradePoint)
    );
    LoadStuff();
}

AbstractScreen* CharactersInfo::Clone() const { return new CharactersInfo; }

AbstractNavigation::NavigationRes CharactersInfo::Update(
    float deltaTime, const AbstractNavigation* navigation
)
{
    auto res = navigation->NoChange();
    if (currentSelect != 0) {
        leftArr.Update(
            deltaTime,
            [&] { audio.PlayHoverSfx(); },
            [&] {
                currentSelect--;
                LoadStuff();
            }
        );
    }
    if (currentSelect != numberOfChars - 1) {
        rightArr.Update(
            deltaTime,
            [&] { audio.PlayHoverSfx(); },
            [&] {
                currentSelect++;
                LoadStuff();
            }
        );
    }
    menu.Update(
        deltaTime,
        [&](uint8_t) noexcept { audio.PlayHoverSfx(); },
        [&](uint8_t selection) noexcept {
            switch (selection) {
                case 0:
                    break;
                case 1:
                    res = navigation->Back();
                    break;
            }
        }
    );
    redraw = lastSelect != currentSelect;
    lastSelect = currentSelect;
    return res;
}

void CharactersInfo::Draw(AbstractCanvas* canvas) const
{
    if (redraw) {
        canvas->Clear(bgColor);
        portrait.Paint(canvas, portraitPos);
        if (currentSelect != 0) {
            leftArr.Draw(canvas);
        }
        if (currentSelect != numberOfChars - 1) {
            rightArr.Draw(canvas);
        }
        Font::DrawString(
            canvas,
            charStuff[currentSelect].Name,
            {XCoord - 12, 10},
            2,
            1,
            fontColor
        );
        Font::DrawStringInBox(
            canvas,
            charStuff[currentSelect].Desc,
            {{XCoord, 50}, {_CanvasSize.width - XCoord - 20, 100}},
            1,
            0,
            fontColor
        );
        Font::DrawString(
            canvas, R.String.CharInfo.Skill, {XCoord, 110}, 1, 0, fontColor
        );
        Font::DrawStringInBox(
            canvas,
            charStuff[currentSelect].Skill,
            {{XCoord, 110 + Font::GetDim(0).height + 5},
             {_CanvasSize.width - XCoord - 20, 30}},
            1,
            0,
            fontColor
        );
        auto statusString = std::format("{} ", R.String.CharInfo.Status);
        if (R.Config.GetCharUpgradeStatus(currentSelect)) {
            statusString += R.String.CharInfo.Upgraded;
        } else {
            if (R.Config.UpgradePoint > 0) {
                statusString += R.String.CharInfo.UpgradeAvail;
            } else {
                statusString += R.String.CharInfo.NoSkillPoint;
            }
        }
        Font::DrawString(canvas, statusString, {XCoord, 155}, 1, 0, fontColor);
    }
    Font::DrawString(canvas, UpgradePointStr, {10, 10}, 1, 0, fontColor);
    menu.Draw(canvas);
}
