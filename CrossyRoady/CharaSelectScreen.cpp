#include "CharaSelectScreen.h"

#include "StringRes.h"

using namespace ConsoleGame;
constexpr auto BGPrimary = (ConsoleGame::Color)12;
constexpr auto BGSecond = (ConsoleGame::Color)11;
const auto* charStuff = (CharStuff*)&R.String.Character;
const auto* charStat = (CharStat*)&R.CharsStat;

const std::wstring_view CharacterSelectScreen::ScreenName()
{
    return L"CharSelect";
}

std::wstring_view CharacterSelectScreen::getName() { return ScreenName(); }

void CharacterSelectScreen::Init(const std::any& args)
{
    surfaces[0].props = {
        .size = {100, 30}, .pos = {15, 20}, .background = (Color)14};
    surfaces[1].props = {
        .size = {100 - 1, 164 - 2},
        .pos = {15, 40},
        .hasBorder = true,
        .background = BGSecond,
        .border = (Color)14};
}

void CharacterSelectScreen::Mount(const std::any& args)
{
    currentPalette.Load(std::format(
        RESOURCE_PATH EXTRA_PATH "{}-sel.hex", fileCharName[selected]
    ));
    ChangeColorPalette(currentPalette);
    charShowCase.Load(std::format(
        RESOURCE_PATH EXTRA_PATH "{}-show.anisprite", fileCharName[selected]
    ));
    charShowCase.Play(true);
    speedIcon.Load(RESOURCE_PATH EXTRA_PATH "speed.sprite");
    heartIcon.Load(std::format(
        RESOURCE_PATH CHARACTER_PATH "{}-health.sprite", fileCharName[selected]
    ));
    charAvaMenu[selected].Load(std::format(
        RESOURCE_PATH EXTRA_PATH "{}-sel.sprite", fileCharName[selected]
    ));

    for (int i = 0; i < R.Config.CharUnlocked; i++) {
        if (i == selected) {
            continue;
        }
        charAvaMenu[i].Load(std::format(
            RESOURCE_PATH EXTRA_PATH "{}-gs.sprite", fileCharName[i]
        ));
    }
    for (int i = R.Config.CharUnlocked; i < numberOfChars; i++) {
        charAvaMenu[i].Load(std::format(
            RESOURCE_PATH EXTRA_PATH "{}-locked.sprite", fileCharName[i]
        ));
    }

    auto tmp = charStuff[selected].Name.find(' ');
    charName = charStuff[selected].Name.substr(tmp + 1);
}

AbstractScreen* CharacterSelectScreen::Clone() const
{
    return new CharacterSelectScreen;
}

AbstractNavigation::NavigationRes CharacterSelectScreen::Update(
    float deltaTime, const AbstractNavigation* navigation
)
{
    charShowCase.AutoUpdateFrame(deltaTime);
    return navigation->NoChange();
}

void CharacterSelectScreen::Draw(AbstractCanvas* canvas) const
{
    canvas->Clear(BGPrimary);
    auto& title = R.String.CharSelect.Title;
    static int titlePos =
        (_CanvasSize.width - title.length() * Font::GetDim(1).width) / 2;
    static const auto fontDim0 = Font::GetDim(0);
    static const auto fontDim1 = Font::GetDim(1);
    Font::DrawString(canvas, title, {titlePos, 5}, 1, 1, (Color)14);
    surfaces[0].Draw(canvas);
    surfaces[1].Draw(canvas);
    Font::DrawString(
        canvas,
        charName,
        {surfaces[0].props.pos.x + (surfaces[0].props.size.width -
                                    int(charName.length()) * fontDim1.width) /
                                       2,
         surfaces[0].props.pos.y +
             (surfaces[0].props.size.height - fontDim1.height) / 2 - 3},
        1,
        1,
        BGPrimary
    );
    auto tmp = charShowCase.GetDim();
    charShowCase.Draw(
        canvas,
        {surfaces[1].props.pos.x +
             (surfaces[1].props.size.width - tmp.width) / 2,
         surfaces[1].props.pos.y + 30}
    );
    auto heartRowPos = Vec2{
        surfaces[1].props.pos.x + 10,
        surfaces[1].props.pos.y + surfaces[1].props.size.height * 2 / 3};
    heartIcon.Draw(canvas, heartRowPos);
    heartRowPos.x += heartIcon.GetDim().width + 5;
    Font::DrawString(
        canvas, R.String.CharSelect.Health, heartRowPos, 1, 0, (Color)14
    );
    heartRowPos.x += R.String.CharSelect.Health.length() * fontDim0.width;
    static auto healthStr = std::to_string(charStat[selected].Health);
    Font::DrawString(canvas, healthStr, heartRowPos, 1, 0, (Color)14);

    auto speedRowPos = Vec2{
        surfaces[1].props.pos.x + 10,
        surfaces[1].props.pos.y + surfaces[1].props.size.height * 2 / 3 + 15};
    heartIcon.Draw(canvas, speedRowPos);
    speedRowPos.x += heartIcon.GetDim().width + 5;
    Font::DrawString(
        canvas, R.String.CharSelect.Speed, speedRowPos, 1, 0, (Color)14
    );
    speedRowPos.x += R.String.CharSelect.Speed.length() * fontDim0.width;
    static auto speedStr = std::to_string(charStat[selected].Speed);
    Font::DrawString(canvas, speedStr, speedRowPos, 1, 0, (Color)14);
}

void CharacterSelectScreen::Unmount()
{
    speedIcon.Unload();
    heartIcon.Unload();
    charShowCase.Unload();
    for (auto& sprite : charAvaMenu) {
        sprite.Unload();
    }
}
