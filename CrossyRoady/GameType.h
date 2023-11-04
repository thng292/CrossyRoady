#pragma once
#include <string_view>

#include "ConsoleGame.h"

namespace GameType {
    enum MobType {
        EASY,
        NORMAL,
        HARD,
    };

    enum CharaType { FAUNA, IRYS, MUMEI, KRONII, SANA, BAE };

    enum MapType { FOREST, CITY, HOUSE, DESERT, SPACE, CASINO };

    enum MapMode { INF, NINF };

    enum SpriteType { ROAD, BLOCK, FLOAT };

    constexpr int CHARA_HEALTH[] = {3, 4, 3, 5, 5, 3};
    constexpr double CHARA_SPEED[] = {120, 120, 120, 120, 120, 120};

    const std::string CHARA_NAME_FILE[] = {
        "fauna", "irys", "mumei", "kronii", "sana", "bae"};

    const std::string MAP_NAME_FILE[] = {
        "forest", "city", "house", "desert", "space", "casino"};

    const std::string MOB_NAME_FILE[] = {"mob1", "mob2", "mob3"};

    const std::string SPRITE_NAME_FILE[] = {"road", "block", "float"};

    struct MobData {
        MobType type;
        ConsoleGame::Vec2 size;
        ConsoleGame::AniSprite anisprite;
    };

    struct MobSprite {
        ConsoleGame::AniSprite MobLeft, MobRight;
    };

    struct RoadData {
        bool isLeftToRight;
        double yPos;
        ConsoleGame::Sprite sprite;
        double mobSpeed;
    };

    struct GameMapSprites {
        ConsoleGame::Sprite blockSprite, floatSprite, roadSprite;
        MobSprite mobSpriteEasy, mobSpriteNormal, mobSpriteHard;
    };

    struct GameMapData {
        MapType mapType;
        CharaType charaType;
        MapMode mapMode;
    };
}  // namespace GameType