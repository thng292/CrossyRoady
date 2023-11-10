#include "CharactersInfo.h"

#include <format>

#include "StringRes.h"

using namespace ConsoleGame;

constexpr Color bgColor = Color(0);
constexpr Color fontColor = Color(1);

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
    portraitPos = {10, (_CanvasSize.height - tmp.height) / 2};
}

CharactersInfo::CharactersInfo()
    : leftArr(
          SurfaceArgs{
              .pos = {5, 112},
              .cornerSize = 10,
              .border = fontColor,
},
          false
      ),
      rightArr(
          SurfaceArgs{
              .pos = {369, 112},
              .cornerSize = 10,
              .border = fontColor,
          },
          true
      )
{
}

const std::wstring_view CharactersInfo::ScreenName() { return L"CharInfo"; }

std::wstring_view CharactersInfo::getName() { return ScreenName(); }

void CharactersInfo::Init(const std::any& args) { LoadStuff(); }

AbstractScreen* CharactersInfo::Clone() const { return new CharactersInfo; }

AbstractNavigation::NavigationRes CharactersInfo::Update(
    float deltaTime, const AbstractNavigation* navigation
)
{
    auto res = navigation->NoChange();
    return res;
}

void CharactersInfo::Draw(AbstractCanvas* canvas) const
{
    canvas->Clear(bgColor);
    portrait.Paint(canvas, portraitPos);
    if (currentSelect != 0) {
        leftArr.Draw(canvas);
    }
    if (currentSelect != numberOfChars) {
        rightArr.Draw(canvas);
    }
}
