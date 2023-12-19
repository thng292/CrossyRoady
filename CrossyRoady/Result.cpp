#include "Result.h"

#include "CharaUnlock.h"
#include "Common.h"
#include "GameUtils.h"
#include "MainMenu.h"
#include "StringRes.h"

using namespace ConsoleGame;
using namespace GameType;
using namespace GameUtils;

constexpr char Black = char(13);
constexpr char White = char(14);
constexpr char Gray = char(15);

const std::wstring_view Result::ScreenName() { return L"Result"; }

std::wstring_view Result::getName() { return ScreenName(); }

void Result::Init(const std::any& args)
{
    if (args.has_value()) {
        gameRes = any_cast<GameResult>(args);
    }

    const auto levelB = R.Config.GetCurrentLevel();
    charaUnlock = CheckCharaUnlock();
    const auto levelA = R.Config.GetCurrentLevel();
    levelUp = levelA != levelB;
    if ((levelA + 1) / 2 + 1 > R.Config.MapUnlocked) {
        mapUnlock = true;
        R.Config.MapUnlocked++;
    } else if (levelUp) {
        R.Config.UpgradePoint++;
        R.Config.UpgradePoint += (levelA == 10);
    }

    menu.Init({90, 190}, {100, 18}, {R.String.Result.PlayAgain, R.String.Next});
    surfaceStat.props = {
        .size = {180, 170},
        .pos = {102, 10},
        .cornerSize = 5,
        .hasBorder = true,
        .background = White,
        .border = Black
    };

    constexpr size_t strLen = 27;
    std::string_view left[] = {
        R.String.Result.PlayTime,
        R.String.Result.Score,
        R.String.Result.DamageTaken,
        R.String.Result.MobCollided,
        R.String.Result.SkillUse,
        R.String.Result.ItemPick,
        R.String.Result.DiffReached
    };

    std::string tmp[] = {"Auto", "Easy", "Normal", "Hard"};
    std::string right[] = {
        SecondsToMMSS(gameRes.time),
        std::to_string(gameRes.score),
        std::to_string(gameRes.damage),
        std::to_string(gameRes.numOfMob),
        std::to_string(gameRes.numOfSkill),
        std::to_string(gameRes.numOfItem),
        tmp[gameRes.diff],
    };

    std::string spacePad = "";
    for (int i = 0; i < data.size(); i++) {
        spacePad.resize(strLen - left[i].size() - right[i].size() - 1, ' ');
        data[i] = std::format("{}:{}{}", left[i], spacePad, right[i]);
    }
}

void Result::Mount(const std::any& args)
{
    resultSfx.Open(RESOURCE_PATH SFX_PATH "result.wav");
    if (R.Config.Sfx) {
        resultSfx.Play();
    };
}

AbstractScreen* Result::Clone() const { return new Result; }

AbstractNavigation::NavigationRes Result::Update(
    float deltaTime, const AbstractNavigation* navigation
)
{
    auto res = navigation->NoChange();
    menu.Update(
        deltaTime,
        [&](uint8_t) noexcept {},
        [&](uint8_t selected) noexcept {
            audio.PlayClickSfx();
            switch (selected) {
                case 0:
                    res = navigation->Back(true);
                    break;
                case 1:
                    if (charaUnlock) {
                        res = navigation->Navigate(
                            CharaUnlock::ScreenName(), gameRes.map
                        );
                    } else {
                        res = navigation->PopBackTo(MainMenu::ScreenName());
                    }
                    break;
            }
        }
    );
    return res;
}

void Result::Draw(AbstractCanvas* canvas) const
{
    DrawStat(canvas);
    menu.Draw(canvas);
}

void Result::Unmount() {}

void Result::DrawStat(ConsoleGame::AbstractCanvas* canvas) const
{
    surfaceStat.Draw(canvas);

    ConsoleGame::Font::DrawString(
        canvas, R.String.Result.Title, {112, 20}, 1, 1, Black
    );
    Vec2 tmp = {112, 50};
    for (int i = 0; i < data.size(); i++) {
        ConsoleGame::Font::DrawString(canvas, data[i], tmp, 1, 0, Black);
        tmp.y += ConsoleGame::Font::GetDim(0).height + 3;
    }
    if (levelUp) {
        ConsoleGame::Font::DrawString(
            canvas, R.String.Result.LevelUp, tmp, 1, 1, Black
        );
        tmp.y += ConsoleGame::Font::GetDim(1).height + 3;
    }

    if (mapUnlock) {
        ConsoleGame::Font::DrawString(
            canvas, R.String.Result.MapUnlock, tmp, 1, 1, Black
        );
        tmp.y += ConsoleGame::Font::GetDim(1).height + 3;
    }
}

bool Result::CheckCharaUnlock()
{
    bool res = false;
    switch (gameRes.map) {
        case FOREST:
            R.Config.ForestXP += gameRes.score;
            if (gameRes.score > R.Config.ForestHighScore) {
                R.Config.ForestHighScore = gameRes.score;
            }
            break;
        case CITY:
            R.Config.CityXP += gameRes.score;
            if (gameRes.score >= charReq[gameRes.diff] &&
                !R.Config.IrysUnlocked) {
                res = true;
                R.Config.IrysUnlocked = 1;
            }
            if (gameRes.score > R.Config.CityHighScore) {
                R.Config.CityHighScore = gameRes.score;
            }
            break;
        case HOUSE:
            R.Config.HouseXP += gameRes.score;
            if (gameRes.score >= charReq[gameRes.diff] &&
                !R.Config.MumeiUnlocked) {
                res = true;
                R.Config.MumeiUnlocked = 1;
            }
            if (gameRes.score > R.Config.HouseHighScore) {
                R.Config.HouseHighScore = gameRes.score;
            }
            break;
        case DESERT:
            R.Config.DesertXP += gameRes.score;
            if (gameRes.score >= charReq[gameRes.diff] &&
                !R.Config.KroniiUnlocked) {
                res = true;
                R.Config.KroniiUnlocked = 1;
            }
            if (gameRes.score > R.Config.DesertHighScore) {
                R.Config.DesertHighScore = gameRes.score;
            }
            break;
        case SPACE:
            R.Config.SpaceXP += gameRes.score;
            if (gameRes.score >= charReq[gameRes.diff] &&
                !R.Config.SanaUnlocked) {
                res = true;
                R.Config.SanaUnlocked = 1;
            }
            if (gameRes.score > R.Config.SpaceHighScore) {
                R.Config.SpaceHighScore = gameRes.score;
            }
            break;
        case CASINO:
            R.Config.CasinoXP += gameRes.score;
            if (gameRes.score >= charReq[gameRes.diff] &&
                !R.Config.BaeUnlocked) {
                res = true;
                R.Config.BaeUnlocked = 1;
            }
            if (gameRes.score > R.Config.CasinoHighScore) {
                R.Config.CasinoHighScore = gameRes.score;
            }
            break;
    }
    return res;
}
