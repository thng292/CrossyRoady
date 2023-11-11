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

    enum LaneType { ROAD, RAIL, WATER, SAFE };

    enum class CollisionType { None, Left, Right, Top, Bottom };

    constexpr int CHARA_HEALTH[] = {3, 4, 3, 5, 5, 3};
    constexpr double CHARA_SPEED[] = {120, 120, 120, 120, 120, 120};

    const std::string CHARA_NAME_FILE[] = {
        "fauna", "irys", "mumei", "kronii", "sana", "bae"};

    const std::string MAP_NAME_FILE[] = {
        "forest", "city", "house", "desert", "space", "casino"};

    const std::string MOB_NAME_FILE[] = {"mob1", "mob2", "mob3"};

    const std::string SPRITE_NAME_FILE[] = {
        "road", "block", "float", "health", "skill", "debuff", "speed", "star"};

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
        ConsoleGame::Sprite blockSprite;
        ConsoleGame::Sprite floatSprite;
        ConsoleGame::Sprite roadSprite;

        ConsoleGame::Sprite emptyHealth;
        ConsoleGame::Sprite health;
        ConsoleGame::Sprite skill;
        ConsoleGame::Sprite skillDebuff;

        ConsoleGame::Sprite debuff;
        ConsoleGame::Sprite debuffExtra;

        ConsoleGame::Sprite itemSpeed;
        ConsoleGame::Sprite itemStar;

        MobSprite mobSpriteEasy;
        MobSprite mobSpriteNormal;
        MobSprite mobSpriteHard;
    };

    struct GameMapData {
        MapType mapType;
        CharaType charaType;
        MapMode mapMode;
    };

    struct GameFlags {
        // Collision
        bool damageCollision: 1;
        bool logCollision : 1;
        bool itemCollision : 1;
        bool blockCollision : 1;

        // Keys
        bool allowKeyLeft: 1;
        bool allowKeyRight : 1;
        bool allowKeyUp: 1;
        bool allowKeyDown: 1;

        // Special
        bool allowSkill: 1;
        bool allowDebuffSkill: 1;

        // Movements
        bool allowMoveLeft: 1;
        bool allowMoveRight : 1;
        bool allowMoveUp: 1;
        bool allowMoveDown : 1;

        // States
        bool isMoving : 1;
        bool justMoved : 1;
        bool movingUp : 1;
        bool movingDown : 1;
        bool movingLeft : 1;
        bool movingRight: 1;
    };

   

  
}  // namespace GameType