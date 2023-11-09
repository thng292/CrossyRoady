#include "Progress.h"

#include "Common.h"
#include "StringRes.h"
#include "TimePlayedTracker.h"

using namespace ConsoleGame;

Progress::Progress()
    : leftArr(
          SurfaceArgs{
              .pos = {200, 70},
              .cornerSize = 5,
              .background = Color(14),
              .border = Color(13)
},
          false
      ),
      rightArr(
          SurfaceArgs{
              .pos = {360, 70},
              .cornerSize = 5,
              .background = Color(14),
              .border = Color(13)},
          true
      )
{
}

const std::wstring_view Progress::ScreenName() { return L"Progress"; }

std::wstring_view Progress::getName() { return ScreenName(); }

int GetCharUpgraded();
uint64_t GetTotalXP();

void Progress::Init(const std::any& args)
{
    bg = std::any_cast<MenuBG*>(args);
    backButt.Init({(_CanvasSize.width - 80) / 2, 200}, {80, 18}, {R.Back});
    surfaceStat.props = {
        .size = {180, 180},
        .pos = {10,  10 },
        .cornerSize = 5,
        .hasBorder = true,
        .background = (Color)14,
        .border = (Color)13
    };
    surfaceExp.props = {
        .size = {180, 180},
        .pos = {194, 10 },
        .cornerSize = 5,
        .hasBorder = true,
        .background = (Color)14,
        .border = (Color)13
    };

    rectPos = {
        Vec2{228, 70},
        Vec2{284, 70},
        Vec2{340, 70}
    };
    rectR = {15, 30, 15};

    constexpr size_t strLen = 27;
    std::string_view left[] = {
        R.Statistic.PlayTime,
        R.Statistic.Deaths,
        R.Statistic.Walked,
        R.Statistic.EarnedXP,
        R.Statistic.CharUnlocked,
        R.Statistic.CharUpgraded,
        R.Statistic.MapUnlocked,
        R.Statistic.Completion};

    int charUnlocked = 1;
    int mapUnlocked = 1;
    earnedXP = GetTotalXP();
    int charUpgraded = GetCharUpgraded();
    try {
        charUnlocked = std::stoi(LocalStorage::Get(R.Config.CharUnlocked));
    } catch (...) {
    }
    try {
        mapUnlocked = std::stoi(LocalStorage::Get(R.Config.MapUnlocked));
    } catch (...) {
    }

    std::string right[] = {
        SecondsToHMStr(TimePlayedTracker::GetTimePlayed()),
        LocalStorage::Get(R.Config.Deaths),
        LocalStorage::Get(R.Config.Walked),
        std::to_string(earnedXP),
        LocalStorage::Get(R.Config.CharUnlocked) + "/6",
        std::format("{}/6", charUpgraded),
        LocalStorage::Get(R.Config.MapUnlocked) + "/6",
        std::format(
            "{:.2f}%",
            float(charUnlocked - 1 + mapUnlocked - 1 + charUpgraded) / 16
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
    bg->Update(deltaTime);
    if (currentLevel != 0) {
        leftArr.Update(
            deltaTime,
            [&] { audio.PlayHoverSfx(); },
            [&] {
                audio.PlayClickSfx();
                currentLevel--;
            }
        );
    }
    if (currentLevel != 10) {
        rightArr.Update(
            deltaTime,
            [&] { audio.PlayHoverSfx(); },
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

    Font::DrawString(canvas, R.Statistic.Title, {20, 20}, 1, 1, (Color)13);
    Vec2 tmp = {20, 60};
    for (int i = 0; i < data.size(); i++) {
        Font::DrawString(canvas, data[i], tmp, 1, 0, (Color)13);
        tmp.y += Font::GetDim(0).height + 3;
    }
}

void Progress::DrawExp(ConsoleGame::AbstractCanvas* canvas) const
{
    surfaceExp.Draw(canvas);
    Font::DrawString(canvas, R.Exp.Level, {204, 20}, 1, 1, (Color)13);
    if (currentLevel != 0) {
        DrawRhombus(canvas, rectPos[0], rectR[0], (Color)15);
        Font::DrawString(
            canvas,
            std::to_string(currentLevel - 1),
            {rectPos[0].x - Font::GetDim(1).x / 2,
             rectPos[0].y - Font::GetDim(1).y / 2},
            1,
            1,
            (Color)14
        );
    }

    auto tmpStr = std::to_string(currentLevel);
    DrawRhombus(canvas, rectPos[1], rectR[1], (Color)13);
    Font::DrawString(
        canvas,
        tmpStr,
        {int(rectPos[1].x - Font::GetDim(1).x * tmpStr.size() - 8),
         rectPos[1].y - Font::GetDim(1).y},
        2,
        1,
        (Color)14
    );
    tmpStr = std::to_string(currentLevel + 1);
    if (currentLevel != 10) {
        DrawRhombus(canvas, rectPos[2], rectR[2], (Color)15);
        Font::DrawString(
            canvas,
            tmpStr,
            {int(rectPos[2].x - Font::GetDim(1).x * tmpStr.size() / 2),
             rectPos[2].y - Font::GetDim(1).y / 2},
            1,
            1,
            (Color)14
        );
    }

    Font::DrawString(
        canvas,
        R.Exp.Rewards,
        {int(284 - Font::GetDim(1).x * R.Exp.Rewards.length() / 2), 110},
        1,
        1,
        (Color)13
    );
    auto reward = R.Exp.UnlockNewMap;
    if (currentLevel % 2 == 0) {
        reward = R.Exp.UnlockUpgradeToken;
    }
    if (currentLevel == 10) {
        reward = R.Exp.Unlock2UpgradeToken;
    }
    Font::DrawString(
        canvas,
        reward,
        {int(284 - Font::GetDim(0).x * reward.length() / 2), 140},
        1,
        0,
        (Color)13
    );
}

int GetCharUpgraded()
{
    int res = 0;
    res += LocalStorage::Get(R.Config.BaeUpgraded) == R.Config.True;
    res += LocalStorage::Get(R.Config.FaunaUpgraded) == R.Config.True;
    res += LocalStorage::Get(R.Config.IrysUpgraded) == R.Config.True;
    res += LocalStorage::Get(R.Config.KroniiUpgraded) == R.Config.True;
    res += LocalStorage::Get(R.Config.MumeiUpgraded) == R.Config.True;
    res += LocalStorage::Get(R.Config.SanaUpgraded) == R.Config.True;
    return res;
}

uint64_t GetTotalXP()
{
    uint64_t res = 0;
    try {
        res += std::stoul(LocalStorage::Get(R.Config.CasinoXP));
    } catch (...) {
    }
    try {
        res += std::stoul(LocalStorage::Get(R.Config.CityXP));
    } catch (...) {
    }
    try {
        res += std::stoul(LocalStorage::Get(R.Config.DesertXP));
    } catch (...) {
    }
    try {
        res += std::stoul(LocalStorage::Get(R.Config.ForestXP));
    } catch (...) {
    }
    try {
        res += std::stoul(LocalStorage::Get(R.Config.HouseXP));
    } catch (...) {
    }
    try {
        res += std::stoul(LocalStorage::Get(R.Config.SpaceXP));
    } catch (...) {
    }
    return res;
}
