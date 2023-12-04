#include "GameUtils.h"

using namespace GameType;
using namespace ConsoleGame;

void GameUtils::LoadMobSprite(
    MapType mapType, MobType mobType, MobSprite& mobSprite
)
{
    std::string pathToMap = GetPathToMap(mapType);
    mobSprite.MobLeft.Load(
        std::format("{}/{}L.anisprite", pathToMap, fileMobName[mobType])
    );
    mobSprite.MobRight.Load(
        std::format("{}/{}R.anisprite", pathToMap, fileMobName[mobType])
    );
    mobSprite.MobLeft.Play(1);
    mobSprite.MobRight.Play(1);
}

void GameUtils::LoadMapSprite(
    MapType mapType, Sprite& sprite, const std::string& src
)
{
    sprite.Load(std::format("{}/{}.sprite", GetPathToMap(mapType), src));
}

void GameUtils::LoadCharaSprite(
    CharaType charaType, Sprite& sprite, const std::string& src
)
{
    sprite.Load(std::format(
        "{}/{}-{}.sprite",
        GetPathToChar(charaType),
        fileCharName[charaType],
        src
    ));
}

Palette GameUtils::GetGamePalette(MapType mapType, CharaType charaType)
{
    auto padColor = RGB(85, 85, 85);
    Palette mapPalette(
        std::format("{}/{}.hex", GetPathToMap(mapType), fileMapName[mapType])
    );
    Palette charaPalette(std::format(
        "{}/{}.hex", GetPathToChar(charaType), fileCharName[charaType]
    ));
    Palette palette;

    auto charaPaletteVal = charaPalette.GetColorPalette();
    auto mapPaletteVal = mapPalette.GetColorPalette();
    for (size_t i = 0; i < 16; ++i) {
        if (charaPaletteVal[i] != padColor) palette[i] = charaPaletteVal[i];
        if (mapPaletteVal[i] != padColor) palette[i] = mapPaletteVal[i];
    }
    return palette;
}

void GameUtils::LoadHeartSprite(
    AniSprite& sprite, GameType::CharaType charaType
)
{
    sprite.Load(std::format(
        "{}{}item-heart-{}.anisprite",
        RESOURCE_PATH,
        EXTRA_PATH,
        fileCharName[charaType]
    ));
}

void GameUtils::DrawHitbox(AbstractCanvas* canvas, Box hitbox, Color color)
{
    int width = hitbox.dim.width;
    int height = hitbox.dim.height;
    int hitX = hitbox.coord.x;
    int hitY = _CONSOLE_HEIGHT_ * 2 - hitbox.coord.y;

    for (int i = 0; i < width; i++) {
        int curX = hitX + i;
        if (curX > 0 && curX < _CONSOLE_WIDTH_) {
            (*canvas)[hitY][curX] = color;
            int bottomY = hitY + height;
            if (bottomY > 0 && bottomY < _CONSOLE_HEIGHT_ * 2) {
                (*canvas)[bottomY][curX] = color;
            }
        }
    }
    for (int i = 0; i < height; i++) {
        int curY = hitY + i;
        if (curY > 0 && curY < _CONSOLE_HEIGHT_ * 2) {
            (*canvas)[curY][hitX] = color;
            int rightX = hitX + width;
            if (rightX > 0 && rightX < _CONSOLE_WIDTH_) {
                (*canvas)[curY][rightX] = color;
            }
        }
    }
}

void GameUtils::DrawTRTriangle(
    ConsoleGame::AbstractCanvas* canvas, uint16_t length, Color color
)
{
    auto startX = _CONSOLE_WIDTH_ - length;
    auto startY = 0;
    auto endY = startY + length;
    auto endX = _CONSOLE_WIDTH_;
    for (auto y = startY; y < endY; ++y) {
        for (auto x = startX; x < endX; ++x) {
            (*canvas)[y][x] = color;
        }
        ++startX;
    }
}

void GameUtils::DrawBLTriangle(
    ConsoleGame::AbstractCanvas* canvas, uint16_t length, Color color
)
{
    auto startX = 0;
    auto startY = _CONSOLE_HEIGHT_ * 2 - length;
    auto endY = startY + length;
    auto endX = 0;
    for (auto y = startY; y < endY; ++y) {
        for (auto x = startX; x < endX; ++x) {
            (*canvas)[y][x] = color;
        }
        ++endX;
    }
}

CollisionType GameUtils::GetCollisionType(Box box1, Box box2)
{
    int box1Left = box1.coord.x;
    int box1Right = box1.coord.x + box1.dim.width;
    int box1Top = box1.coord.y;
    int box1Bottom = box1.coord.y - box1.dim.height;

    int box2Left = box2.coord.x;
    int box2Right = box2.coord.x + box2.dim.width;
    int box2Top = box2.coord.y;
    int box2Bottom = box2.coord.y - box2.dim.height;

    int thres = 3;

    if (box1Left <= box2Right && box1Right >= box2Left &&
        box1Top >= box2Bottom && box1Bottom <= box2Top) {
        // Calculate overlap in x and y dimensions
        int minDistX = std::min(
            std::abs(box1Right - box2Left), std::abs(box1Left - box2Right)
        );
        int minDistY = std::min(
            std::abs(box1Bottom - box2Top), std::abs(box1Top - box2Bottom)
        );
        bool isRightIn = box1Right >= box2Left && box1Right <= box2Right;
        bool isBottomIn = box1Bottom >= box2Bottom && box1Bottom <= box2Top;
        bool isLeftIn = box1Left <= box2Right && box1Left >= box2Left;
        bool isTopIn = box1Top >= box2Bottom && box1Top <= box2Top;
        if (minDistX >= thres && minDistY >= thres) {
            if (isRightIn && isTopIn) {
                return BottomLeft;
            }
            if (isLeftIn && isTopIn) {
                return BottomRight;
            }
            if (isRightIn && isBottomIn) {
                return TopLeft;
            }

            if (isLeftIn && isBottomIn) {
                return TopRight;
            }
        }

        // Determine the type of collision based on overlap
        if (minDistX <= minDistY) {
            bool isRightIn = box1Right >= box2Left && box1Right <= box2Right;
            if (isRightIn) {
                return Left;
            } else {
                return Right;
            }
        } else {
            bool isBottomIn = box1Bottom >= box2Bottom && box1Bottom <= box2Top;
            if (isBottomIn) {
                return Top;
            } else {
                return Bottom;
            }
        }
    }
    return None;
}

std::string GameUtils::SecondsToMMSS(float time)
{
    auto min = (int)time / 60;
    auto sec = (int)time % 60;
    std::string minStr =
        (min < 10) ? "0" + std::to_string(min) : std::to_string(min);
    std::string secStr =
        (sec < 10) ? "0" + std::to_string(sec) : std::to_string(sec);
    return std::format("{}:{}", minStr, secStr);
}

float GameUtils::GetDistance(int x1, int y1, int x2, int y2)
{
    return std::hypot(x2 - x1, y2 - y1);
}

std::string GameUtils::GetPathToMap(MapType mapType)
{
    std::string path =
        std::format("{}{}{}", RESOURCE_PATH, MAP_PATH, fileMapName[mapType]);
    return path;
}

std::string GameUtils::GetPathToChar(CharaType charaType)
{
    std::string path = std::format("{}{}", RESOURCE_PATH, CHARACTER_PATH);
    return path;
}

GameMapData GameUtils::GetGMData(GameType::UserOption userOpt)
{
    GameMapData gm;
    gm.charaType = static_cast<CharaType>(userOpt.character);
    gm.mapType = static_cast<MapType>(userOpt.map);
    gm.enableDebuff = userOpt.debuff;
    gm.mapMode = userOpt.isTimed ? NINF : INF;
    gm.mapDifficulty = static_cast<MapDifficulty>(userOpt.difficulty);
    gm.music = userOpt.music;
    gm.time = userOpt.time;

    return gm;
}
