#pragma once
#include <math.h>

#include <format>
#include <fstream>

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

    void LoadHeartSprite(
        ConsoleGame::AniSprite& sprite, GameType::CharaType charaType
    );

    ConsoleGame::Palette GetGamePalette(
        GameType::MapType mapType, GameType::CharaType charaType
    );

    void DrawHitbox(
        ConsoleGame::AbstractCanvas* canvas,
        ConsoleGame::Box hitbox,
        ConsoleGame::Color color = ConsoleGame::Color::BLACK
    );

    void DrawTRTriangle(
        ConsoleGame::AbstractCanvas* canvas,
        uint16_t length,
        ConsoleGame::Color = (ConsoleGame::Color)10
    );
    void DrawBLTriangle(
        ConsoleGame::AbstractCanvas* canvas,
        uint16_t length,
        ConsoleGame::Color = (ConsoleGame::Color)10
    );

    GameType::CollisionType GetCollisionType(
        ConsoleGame::Box box1, ConsoleGame::Box box2
    );

    std::string SecondsToMMSS(float time);

    float GetDistance(int x1, int y1, int x2, int y2);
    std::string GetPathToMap(GameType::MapType mapType);
    std::string GetPathToChar(GameType::CharaType charaType);

}  // namespace GameUtils