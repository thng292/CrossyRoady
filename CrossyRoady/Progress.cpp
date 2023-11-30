#include "Progress.h"

#include "Common.h"
#include "StringRes.h"
#include "TimePlayedTracker.h"

using namespace ConsoleGame;

constexpr Color Black = Color(13);
constexpr Color White = Color(14);
constexpr Color Gray = Color(15);

Progress::Progress()
    : leftArr(
          SurfaceArgs{
              .pos = {200, 70 - 8},
              .cornerSize = 8,
              .background = White,
              .border = Black},
          false
      ),
      rightArr(
          SurfaceArgs{
              .pos = {360, 70 - 8},
              .cornerSize = 8,
              .background = White,
              .border = Black},
          true
      )
{
}

const std::wstring_view Progress::ScreenName() { return L"Progress"; }

std::wstring_view Progress::getName() { return ScreenName(); }

void Progress::Init(const std::any& args)
{
    bg = std::any_cast<MenuBG*>(args);
    backButt.Init(
        {(_CanvasSize.width - 80) / 2, 200}, {80, 18}, {R.String.Back}
    );
    surfaceStat.props = {
        .size = {180, 180},
        .pos = {10, 10},
        .cornerSize = 5,
        .hasBorder = true,
        .background = White,
        .border = Black};
    surfaceExp.props = {
        .size = {180, 180},
        .pos = {194, 10},
        .cornerSize = 5,
        .hasBorder = true,
        .background = White,
        .border = Black};

    rectPos = {Vec2{228, 70}, Vec2{284, 70}, Vec2{340, 70}};
    rectR = {15, 30, 15};

    constexpr size_t strLen = 27;
    std::string_view left[] = {
        R.String.Progress.PlayTime,
        R.String.Progress.Deaths,
        R.String.Progress.Walked,
        R.String.Progress.EarnedXP,
        R.String.Progress.Level,
        R.String.Progress.CharUnlocked,
        R.String.Progress.CharUpgraded,
        R.String.Progress.MapUnlocked,
        R.String.Progress.Completion};

    currentLevel = R.Config.GetCurrentLevel();
    earnedXP = R.Config.GetTotalXP();
    int charUpgraded = R.Config.GetCharUpgraded();

    std::string right[] = {
        SecondsToHMStr(TimePlayedTracker::GetTimePlayed()),
        std::to_string(R.Config.Deaths),
        std::to_string(R.Config.Walked),
        std::to_string(earnedXP),
        std::to_string(R.Config.GetCurrentLevel()),
        std::to_string(R.Config.GetCharUnlocked()) + "/6",
        std::format("{}/6", charUpgraded),
        std::to_string(R.Config.MapUnlocked) + "/6",
        std::format(
            "{:.2f}%",
            float(
                R.Config.GetCharUnlocked() - 1 + R.Config.MapUnlocked - 1 +
                charUpgraded
            ) / 16
        ),
    };
    std::string spacePad = "";
    for (int i = 0; i < data.size(); i++) {
        spacePad.resize(strLen - left[i].size() - right[i].size() - 1, ' ');
        data[i] = std::format("{}:{}{}", left[i], spacePad, right[i]);
    }
}

void Progress::Mount(const std::any& args)
{
    leftArr.Mount();
    rightArr.Mount();
}

AbstractScreen* Progress::Clone() const { return new Progress; }

AbstractNavigation::NavigationRes Progress::Update(
    float deltaTime, const AbstractNavigation* navigation
)
{
    auto res = navigation->NoChange();

    //leftArr.ChangeColor(White, Black);
    //rightArr.ChangeColor(White, Black);
    bg->Update(deltaTime);
    if (currentLevel != 0) {
        leftArr.Update(
            deltaTime,
            [&] {
                audio.PlayHoverSfx();
                //leftArr.ChangeColor(Black, Black);
            },
            [&] {
                audio.PlayClickSfx();
                currentLevel--;
            }
        );
    }
    if (currentLevel != 10) {
        rightArr.Update(
            deltaTime,
            [&] {
                audio.PlayHoverSfx();
                //rightArr.ChangeColor(Black, Black);
            },
            [&] {
                audio.PlayClickSfx();
                currentLevel++;
            }
        );
    }
    backButt.Update(
        deltaTime,
        [&](uint8_t) noexcept {},
        [&](uint8_t) noexcept {
            audio.PlayClickSfx();
            res = navigation->Back();
        }
    );
    return res;
}

void Progress::Draw(AbstractCanvas* canvas) const
{
    bg->Draw(canvas);

    DrawStat(canvas);
    DrawExp(canvas);
    if (currentLevel != 0) {
        leftArr.Draw(canvas);
    }
    if (currentLevel != 10) {
        rightArr.Draw(canvas);
    }

    backButt.Draw(canvas);
}

void Progress::Unmount() {}

void Progress::DrawStat(ConsoleGame::AbstractCanvas* canvas) const
{
    surfaceStat.Draw(canvas);

    Font::DrawString(canvas, R.String.Progress.Stat_Title, {20, 20}, 1, 1, Black);
    Vec2 tmp = {20, 60};
    for (int i = 0; i < data.size(); i++) {
        Font::DrawString(canvas, data[i], tmp, 1, 0, Black);
        tmp.y += Font::GetDim(0).height + 3;
    }
}

void Progress::DrawExp(ConsoleGame::AbstractCanvas* canvas) const
{
    surfaceExp.Draw(canvas);
    Font::DrawString(canvas, R.String.Progress.Level, {204, 20}, 1, 1, Black);
    if (currentLevel != 0) {
        DrawRhombus(canvas, rectPos[0], rectR[0], Gray);
        Font::DrawString(
            canvas,
            std::to_string(currentLevel - 1),
            {rectPos[0].x - Font::GetDim(1).x / 2,
             rectPos[0].y - Font::GetDim(1).y / 2},
            1,
            1,
            White
        );
    }

    auto tmpStr = std::to_string(currentLevel);
    DrawRhombus(canvas, rectPos[1], rectR[1], Black);
    Font::DrawString(
        canvas,
        tmpStr,
        {int(rectPos[1].x - Font::GetDim(1).x * tmpStr.size() - 8),
         rectPos[1].y - Font::GetDim(1).y},
        2,
        1,
        White
    );
    tmpStr = std::to_string(currentLevel + 1);
    if (currentLevel != 10) {
        DrawRhombus(canvas, rectPos[2], rectR[2], Gray);
        Font::DrawString(
            canvas,
            tmpStr,
            {int(rectPos[2].x - Font::GetDim(1).x * tmpStr.size() / 2),
             rectPos[2].y - Font::GetDim(1).y / 2},
            1,
            1,
            White
        );
    }

    Font::DrawString(
        canvas,
        R.String.Progress.Rewards,
        {int(284 - Font::GetDim(1).x * R.String.Progress.Rewards.length() / 2),
         110},
        1,
        1,
        Black
    );
    auto reward = R.String.Progress.UnlockNewMap;
    if (currentLevel % 2 == 0) {
        reward = R.String.Progress.UnlockUpgradeToken;
    }
    if (currentLevel == 0) {
        reward = "";
    }
    if (currentLevel == 10) {
        reward = R.String.Progress.Unlock2UpgradeToken;
    }
    Font::DrawString(
        canvas,
        reward,
        {int(284 - Font::GetDim(0).x * reward.length() / 2), 140},
        1,
        0,
        Black
    );
}
