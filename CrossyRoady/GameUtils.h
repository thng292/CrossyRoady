#pragma once
#include <format>

#include "ConsoleGame.h"
#include "GameType.h"
#include "StringRes.h"

namespace GameUtils {
    void LoadMobSprite(
        GameType::MapType mapType,
        GameType::MobType mobType,
        GameType::MobSprite& mobSprite
    );

    void LoadStaticSprite(
        GameType::MapType mapType,
        GameType::SpriteType spriteType,
        ConsoleGame::Sprite& sprite
    );

    ConsoleGame::Palette GetGamePalette(
        GameType::MapType mapType, GameType::CharaType charaType
    );

    std::string GetPathToMap(GameType::MapType mapType);

    std::string GetPathToChar(GameType::CharaType charaType);
}  // namespace GameUtils