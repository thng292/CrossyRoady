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

void GameUtils::LoadStaticSprite(
    GameType::MapType mapType,
    GameType::SpriteType spriteType,
    ConsoleGame::Sprite& sprite
)
{
    sprite.Load(std::format(
        "{}/{}.sprite",
        GetPathToMap(mapType),
        GameType::SPRITE_NAME_FILE[spriteType]
    ));
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
