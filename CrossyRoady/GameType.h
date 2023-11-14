#pragma once
#include <string_view>

#include "ConsoleGame.h"

namespace GameType {

    // Map values
    constexpr int DAMAGE_COOLDOWN = 3;
    constexpr int MAP_SPEED = 0;

    // Debuff values
    constexpr int MAP_DEBUFF_COOLDOWN = 30;
    constexpr int FAUNA_DEBUFF_DURATION = 10;
    constexpr int IRYS_DEBUFF_DURATION = 10;
    constexpr int MUMEI_DEBUFF_DURATION = 10;
    constexpr int KRONII_DEBUFF_DURATION = 10;
    constexpr int SANA_DEBUFF_DURATION = 30;
    constexpr int BAE_DEBUFF_DURATION = 10;

    constexpr int MAX_IDLE_TIME = 1;
    constexpr int IRYS_DEBUFF_HEALTH = 1;
    constexpr int VISIBLE_RADIUS = 30;
    constexpr int MUMEI_VISIBLE_RADIUS = 100;

    // Skill values
    constexpr int SKILL_DURATION = 5;
    constexpr int FAUNA_MAX_HEALTH = 3;
    constexpr int FAUNA_EXTRA_MAX_HEALTH = FAUNA_MAX_HEALTH + 2;
    constexpr int IRYS_SHIELD_COUNT = 2;
    constexpr int MUMEI_SPEED_BUFF = 50;

    enum MobType {
        EASY,
        NORMAL,
        HARD,
    };

    enum CharaType { FAUNA, IRYS, MUMEI, KRONII, SANA, BAE };

    enum MapType { FOREST, CITY, HOUSE, DESERT, SPACE, CASINO };

    enum MapMode { INF, NINF };

    enum LaneType { ROAD, RAIL, WATER, SAFE };

    enum CollisionType { None, Left, Right, Top, Bottom };

    enum SkillCategory { TIME, SHIELD };

    constexpr int CHARA_HEALTH[] = {3, 4, 3, 5, 5, 3};
    constexpr double CHARA_SPEED[] = {120, 120, 120, 120, 120, 120};
    constexpr float DEBUFF_DURATION[] = {10, 10, 10, 10, 30, 10};

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
        ConsoleGame::Sprite safeSprite;
        ConsoleGame::Sprite waterSprite;

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
        bool damageCollision : 1 = false;
        bool logCollision : 1 = false;
        bool itemCollision : 1 = false;
        bool blockCollision : 1 = false;

        // Keys
        bool allowMovementKeys : 1 = true;
        bool allowSkillKey : 1 = true;
        bool allowKeyLeft : 1 = true;
        bool allowKeyRight : 1 = true;
        bool allowKeyUp : 1 = true;
        bool allowKeyDown : 1 = true;

        // Special
        bool allowSkill : 1 = true;
        bool allowDebuff : 1 = true;
        bool allowLaneUpdate : 1 = true;

        // Movements
        bool allowMoveLeft : 1 = true;
        bool allowMoveRight : 1 = true;
        bool allowMoveUp : 1 = true;
        bool allowMoveDown : 1 = true;

        // Character states
        bool isMoving : 1 = false;
        bool justMoved : 1 = false;
        bool movingUp : 1 = false;
        bool movingDown : 1 = false;
        bool movingLeft : 1 = false;
        bool movingRight : 1 = false;
        bool isDamageCooldown : 1 = false;

        // Map events
        bool debuffInUse : 1 = false;
        bool debuffCalled : 1 = false;

        bool skillCalled : 1 = false;
        bool skillActivate : 1 = false;
        bool skillInUse : 1 = false;
        bool turnOffSkill : 1 = false;

        bool isInvincible : 1 = false;

        // Action
        bool isDarkMap : 1 = false;
        bool isReverseKey : 1 = false;
    };

    struct GameEventsArgs {
        MobType collidedMobtype;
        MapType debuffType;
        CharaType skillType;
        SkillCategory skillCategory;

        // Cooldown
        float damageCooldownTime = DAMAGE_COOLDOWN;
        float mapDebuffCooldownTime;

        // Duration
        float mapDebuffTime;
        float skillTime = SKILL_DURATION;

        // Miscel
        float notMovingTime = 0;
        float originalHealth = 0;
        float originalSpeed = 0;

        // Skill
        float skillCharge = 0;
        int shield = 0;
    };

}  // namespace GameType