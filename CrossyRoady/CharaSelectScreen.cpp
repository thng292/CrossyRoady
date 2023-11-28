#include "CharaSelectScreen.h"

#include "GameType.h"
#include "GameUtils.h"
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
    constexpr auto panelWidth = 100;
    surfaces[0].props = {
        .size = {panelWidth, 30}, .pos = {15, 30}, .background = (Color)14};
    surfaces[1].props = {
        .size = {panelWidth - 1, 160 - 2},
        .pos = {15, 50},
        .hasBorder = true,
        .background = BGSecond,
        .border = (Color)14};
    surfaces[2].props = {
        .size = {panelWidth, 30},
        .pos = {_CanvasSize.width - 15 - panelWidth, 30},
        .background = (Color)14};
    surfaces[3].props = {
        .size = {panelWidth - 1, 160 - 2},
        .pos = {_CanvasSize.width - 15 - panelWidth, 50},
        .hasBorder = true,
        .background = BGSecond,
        .border = (Color)14};
    backButton = ArrowButton({.pos = {15, 5}, .cornerSize = 8}, false);
}

void CharacterSelectScreen::LoadRes(bool fresh)
{
    currentPalette.Load(std::format(
        RESOURCE_PATH EXTRA_PATH "{}-sel.hex", fileCharName[selected]
    ));
    ChangeColorPalette(currentPalette);
    charShowCase.Load(std::format(
        RESOURCE_PATH EXTRA_PATH "{}-show.anisprite", fileCharName[selected]
    ));
    charShowCase.ResetFrame();
    charShowCase.Play(true);
    speedIcon.Load(RESOURCE_PATH EXTRA_PATH "speed.sprite");
    heartIcon.Load(std::format(
        RESOURCE_PATH EXTRA_PATH "{}-health.sprite", fileCharName[selected]
    ));
    skillIcon.Load(std::format(
        RESOURCE_PATH EXTRA_PATH "{}-skill.sprite", fileCharName[selected]
    ));
    charAvaMenu[selected].Load(std::format(
        RESOURCE_PATH EXTRA_PATH "{}-sel.sprite", fileCharName[selected]
    ));

    if (fresh) {
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
    } else {
        charAvaMenu[lastSelected].Load(std::format(
            RESOURCE_PATH EXTRA_PATH "{}-gs.sprite", fileCharName[lastSelected]
        ));
    }

    auto tmp = charStuff[selected].Name.find(' ');
    charName = charStuff[selected].Name.substr(tmp + 1);
}

void CharacterSelectScreen::Mount(const std::any& args) { LoadRes(true); }

AbstractScreen* CharacterSelectScreen::Clone() const
{
    return new CharacterSelectScreen;
}

AbstractNavigation::NavigationRes CharacterSelectScreen::Update(
    float deltaTime, const AbstractNavigation* navigation
)
{
    charShowCase.AutoUpdateFrame(deltaTime);
    auto mpos = GetMousePos();
    auto inBox = [](Vec2 pos, Vec2 mpos) {
        return mpos.x >= pos.x and mpos.x <= pos.x + 56 and mpos.y >= pos.y and
               mpos.y <= pos.y + 56;
    };
    for (int i = 0; i < R.Config.CharUnlocked; i++) {
        if (inBox(charAvaPos[i], mpos)) {
            selected = i;
        }
    }

    if (UiIsKeyMeanUp()) {
        if (selected < 2) {
            isBackButtSelected = true;
        } else {
            isBackButtSelected = false;
            if (selected - 2 >= 0) {
                selected -= 2;
            }
        }
    }
    if (UiIsKeyMeanDown()) {
        if (selected + 2 < R.Config.CharUnlocked and not isBackButtSelected) {
            selected += 2;
        } else {
            isBackButtSelected = false;
        }
    }
    if (UiIsKeyMeanLeft()) {
        if (selected - 1 >= 0 and not isBackButtSelected) {
            selected -= 1;
        } else {
            isBackButtSelected = false;
        }
    }
    if (UiIsKeyMeanRight()) {
        if (selected + 1 < R.Config.CharUnlocked and not isBackButtSelected) {
            selected += 1;
        } else {
            isBackButtSelected = false;
        }
    }
    if (UiIsKeyMeanSelect()) {
        audio.PlayClickSfx();
        if (isBackButtSelected) {
            return navigation->Back();
        } else {
            return navigation->Navigate(
                L"tmp", GameType::UserOption{.character = selected}
            );
        }
    }
    if (UiIsKeyMeanBack()) {
        return navigation->Back();
    }
    if (selected != lastSelected) {
        LoadRes(false);
        lastSelected = selected;
        audio.PlayHoverSfx();
    }
    if (backButton.IsHover(GetMousePos()) or isBackButtSelected) {
        if (not backButtLastHover) {
            backButtLastHover = true;
            audio.PlayHoverSfx();
        }
        backButton.ChangeColor(BGPrimary, (Color)14);
        if (UiIsKeyMeanClick()) {
            return navigation->Back();
        }
    } else {
        backButtLastHover = false;
        backButton.ChangeColor((Color)14, (Color)14);
    }
    return navigation->NoChange();
}

void CharacterSelectScreen::Draw(AbstractCanvas* canvas) const
{
    canvas->Clear(BGPrimary);
    GameUtils::DrawBLTriangle(canvas, 70);
    GameUtils::DrawTRTriangle(canvas, 70);
    auto& title = R.String.CharSelect.Title;
    static int titlePos =
        (_CanvasSize.width - title.length() * Font::GetDim(1).width) / 2;
    Font::DrawString(canvas, title, {titlePos, 5}, 1, 1, (Color)14);
    for (const auto& surface : surfaces) {
        surface.Draw(canvas);
    }
    DrawLeftPanel(canvas);
    DrawRightPanel(canvas);
    for (int i = 0; i < charAvaMenu.size(); i++) {
        charAvaMenu[i].Draw(canvas, charAvaPos[i]);
    }
    backButton.Draw(canvas);
    // Font::DrawString(canvas, R.String.Back, {25, 7}, 1, 0, (Color)14);
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

void CharacterSelectScreen::DrawLeftPanel(AbstractCanvas* canvas) const
{
    const auto fontDim0 = Font::GetDim(0);
    const auto fontDim1 = Font::GetDim(1);
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
         surfaces[1].props.pos.y + 20}
    );

    auto heartRowPos = Vec2{
        surfaces[1].props.pos.x + 10,
        surfaces[1].props.pos.y + surfaces[1].props.size.height * 2 / 3};
    heartIcon.Draw(canvas, heartRowPos);
    heartRowPos.x += 25;
    heartRowPos.y += (heartIcon.GetDim().height - fontDim0.height) / 2;
    Font::DrawString(
        canvas, R.String.CharSelect.Health, heartRowPos, 1, 0, (Color)14
    );
    heartRowPos.x += R.String.CharSelect.Health.length() * fontDim0.width;
    static auto healthStr = std::to_string(charStat[selected].Health);
    Font::DrawString(canvas, healthStr, heartRowPos, 1, 0, (Color)14);

    for (int i = surfaces[1].props.pos.x + 20;
         i < surfaces[1].props.pos.x + surfaces[1].props.size.width - 20;
         i++) {
        (*canvas)[heartRowPos.y + 16][i] = (Color)14;
    }

    auto speedRowPos = Vec2{
        surfaces[1].props.pos.x + 10,
        surfaces[1].props.pos.y + surfaces[1].props.size.height * 2 / 3 + 25};
    speedIcon.Draw(canvas, speedRowPos);
    speedRowPos.x += 25;
    speedRowPos.y += (speedIcon.GetDim().height - fontDim0.height) / 2;
    Font::DrawString(
        canvas, R.String.CharSelect.Speed, speedRowPos, 1, 0, (Color)14
    );
    speedRowPos.x += R.String.CharSelect.Speed.length() * fontDim0.width;
    static auto speedStr = std::to_string(charStat[selected].Speed);
    Font::DrawString(canvas, speedStr, speedRowPos, 1, 0, (Color)14);
}

void CharacterSelectScreen::DrawRightPanel(AbstractCanvas* canvas) const
{
    const auto fontDim0 = Font::GetDim(0);
    const auto fontDim1 = Font::GetDim(1);
    Font::DrawString(
        canvas,
        R.String.CharSelect.Skill,
        {surfaces[2].props.pos.x +
             (surfaces[2].props.size.width -
              int(R.String.CharSelect.Skill.length()) * fontDim1.width) /
                 2,
         surfaces[2].props.pos.y +
             (surfaces[2].props.size.height - fontDim1.height) / 2 - 3},
        1,
        1,
        BGPrimary
    );
    auto tmp = skillIcon.GetDim();
    skillIcon.Draw(
        canvas,
        {surfaces[3].props.pos.x +
             (surfaces[3].props.size.width - tmp.width) / 2,
         surfaces[3].props.pos.y + 20}
    );
    Font::DrawStringInBox(
        canvas,
        charStuff[selected].Skill,
        {{surfaces[3].props.pos.x + 10, surfaces[3].props.pos.y + 60},
         {surfaces[3].props.size.width - 20, surfaces[3].props.size.height}},
        1,
        0,
        (Color)14
    );
}
