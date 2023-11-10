#include "GameUtils.h"

void GameUtils::LoadMobSprite(
    GameType::MapType mapType,
    GameType::MobType mobType,
    GameType::MobSprite& mobSprite
)
{
    std::string pathToMap = GetPathToMap(mapType);
    mobSprite.MobLeft.Load(std::format(
        "{}/{}L.anisprite", pathToMap, GameType::MOB_NAME_FILE[mobType]
    ));
    mobSprite.MobRight.Load(std::format(
        "{}/{}R.anisprite", pathToMap, GameType::MOB_NAME_FILE[mobType]
    ));
}

void GameUtils::LoadMapSprite(
    GameType::MapType mapType,
    ConsoleGame::Sprite& sprite,
    const std::string& src
)
{
    sprite.Load(std::format("{}/{}.sprite", GetPathToMap(mapType), src));
}

void GameUtils::LoadCharaSprite(
    GameType::CharaType charaType,
    ConsoleGame::Sprite& sprite,
    const std::string& src
)
{
    sprite.Load(std::format(
        "{}/{}-{}.sprite",
        GetPathToChar(charaType),
        GameType::CHARA_NAME_FILE[charaType],
        src
    ));
}

void GameUtils::LoadExtraSprite(
    ConsoleGame::Sprite& sprite, const std::string& src
)
{
    sprite.Load(std::format("{}{}{}.sprite", RESOURCE_PATH, EXTRA_PATH, src));
}

ConsoleGame::Palette GameUtils::GetGamePalette(
    GameType::MapType mapType, GameType::CharaType charaType
)
{
    auto padColor = RGB(85, 85, 85);
    ConsoleGame::Palette mapPalette(std::format(
        "{}/{}.hex", GetPathToMap(mapType), GameType::MAP_NAME_FILE[mapType]
    ));
    ConsoleGame::Palette charaPalette(std::format(
        "{}/{}.hex",
        GetPathToChar(charaType),
        GameType::CHARA_NAME_FILE[charaType]
    ));
    ConsoleGame::Palette palette;

    auto charaPaletteVal = charaPalette.GetColorPalette();
    auto mapPaletteVal = mapPalette.GetColorPalette();
    for (size_t i = 0; i < 16; ++i) {
        if (charaPaletteVal[i] != padColor) palette[i] = charaPaletteVal[i];
        if (mapPaletteVal[i] != padColor) palette[i] = mapPaletteVal[i];
    }
    return palette;
}

void GameUtils::DrawHitbox(
    ConsoleGame::AbstractCanvas* canvas,
    ConsoleGame::Box hitbox,
    ConsoleGame::Color color
)
{
    int width = hitbox.dim.width;
    int height = hitbox.dim.height;
    int hitX = hitbox.coord.x;
    int hitY = ConsoleGame::_CONSOLE_HEIGHT_ * 2 - hitbox.coord.y;

    for (int i = 0; i < width; i++) {
        int curX = hitX + i;
        if (curX > 0 && curX < ConsoleGame::_CONSOLE_WIDTH_) {
            (*canvas)[hitY][curX] = color;
            int bottomY = hitY + height;
            if (bottomY > 0 && bottomY < ConsoleGame::_CONSOLE_HEIGHT_ * 2) {
                (*canvas)[bottomY][curX] = color;
            }
        }
    }
    for (int i = 0; i < height; i++) {
        int curY = hitY + i;
        if (curY > 0 && curY < ConsoleGame::_CONSOLE_HEIGHT_ * 2) {
            (*canvas)[curY][hitX] = color;
            int rightX = hitX + width;
            if (rightX > 0 && rightX < ConsoleGame::_CONSOLE_WIDTH_) {
                (*canvas)[curY][rightX] = color;
            }
        }
    }
}

bool GameUtils::IsCollide(ConsoleGame::Box box1, ConsoleGame::Box box2)
{
    int box1Left = box1.coord.x;
    int box1Right = box1.coord.x + box1.dim.width;
    int box1Top = box1.coord.y;
    int box1Bottom = box1.coord.y + box1.dim.height;

    int box2Left = box2.coord.x;
    int box2Right = box2.coord.x + box2.dim.width;
    int box2Top = box2.coord.y;
    int box2Bottom = box2.coord.y + box2.dim.height;

    if (box1Left <= box2Right && box1Right >= box2Left &&
        box1Top <= box2Bottom && box1Bottom >= box2Top) {
        return true;
    }

    return false;
}

std::string GameUtils::GetPathToMap(GameType::MapType mapType)
{
    std::string path = std::format(
        "{}{}{}", RESOURCE_PATH, MAP_PATH, GameType::MAP_NAME_FILE[mapType]
    );
    return path;
}

std::string GameUtils::GetPathToChar(GameType::CharaType charaType)
{
    std::string path = std::format("{}{}", RESOURCE_PATH, CHARACTER_PATH);
    return path;
}
