#pragma once
#include <string_view>

#include "ConsoleGame.h"

namespace GameType {
    enum MobType {
        EASY = 1,
        NORMAL = 2,
        HARD = 3,
    };

    enum CharaType { FAUNA, IRYS, MUMEI, KRONII, SANA, BAE };

    const int CHARA_HEALTH[] = {3, 4, 3, 5, 5, 3};
    const double CHARA_SPEED[] = {4, 4, 4, 3, 3, 5};
    const std::string_view CHARA_NAME_FILE[] = {
        "fauna", "irys", "mumei", "kronii", "bae"};

    struct MobData {
        MobType type;
        ConsoleGame::Vec2 size;
        ConsoleGame::AniSprite anisprite;
    };

    struct MobSprite {
        ConsoleGame::AniSprite MobUp, MobDown, MobLeft, MobRight;
    };

    struct RoadData {
        bool isLeftToRight;
        double yPos;
        ConsoleGame::Sprite sprite;
        double mobSpeed;
    };

    struct GameMapArgs {
        ConsoleGame::Sprite blockSprite, logSprite, roadSprite;
        MobSprite mobSpriteEasy, mobSpriteNormal, mobSpriteHard;
        CharaType charaType;
    };

    struct GameMapData {};
}  // namespace GameType