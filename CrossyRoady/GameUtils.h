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

    void LoadMapSprite(
        GameType::MapType mapType,
        ConsoleGame::Sprite& sprite,
        const std::string& src
    );

    void LoadCharaSprite(
        GameType::CharaType charaType,
        ConsoleGame::Sprite& sprite,
        const std::string& src
    );

    void LoadExtraSprite(ConsoleGame::Sprite& sprite, const std::string& src);

    ConsoleGame::Palette GetGamePalette(
        GameType::MapType mapType, GameType::CharaType charaType
    );

    std::string GetPathToMap(GameType::MapType mapType);
    std::string GetPathToChar(GameType::CharaType charaType);
}  // namespace GameUtils