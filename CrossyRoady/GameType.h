#pragma once
#include <string_view>

#include "ConsoleGame.h"

namespace GameType {

    // Map values
    constexpr int DAMAGE_COOLDOWN = 3;
    constexpr int MAP_DRAG_SPEED = 20.0f;
    constexpr float ITEM_SPAWN_RATE = 1.0f;
    constexpr float RAIL_SPAWN_RATE = 0.1f;
    constexpr float SPEED_ADDITION = 3.0f;
    constexpr float MIN_DIST_SCORE = 32.0f;
    constexpr float MIN_DIST_CHARGE = 20.0f;

    constexpr int MAX_SKILL_CHARGE = 100;

    // Debuff values
    constexpr int MAP_DEBUFF_COOLDOWN = 30;

    constexpr int MAX_IDLE_TIME = 3;
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

    enum Direction { LEFT, RIGHT, UP, DOWN };

    enum CharaType { FAUNA, IRYS, MUMEI, KRONII, SANA, BAE };

    enum MapType { FOREST, CITY, HOUSE, DESERT, SPACE, CASINO };

    enum MapMode { INF, NINF };

    enum MapDifficulty { MPROG, MEASY, MNORMAL, MHARD };

    enum LaneType { ROAD, WATER, SAFE, RAIL };

    enum CollisionType { None, Left, Right, Top, Bottom };

    enum SkillCategory { TIME, SHIELD };

    enum ItemType { SPEED, STAR, HEALTH };

    constexpr int CHARA_HEALTH[] = {3, 4, 3, 5, 5, 3};
    constexpr double CHARA_SPEED[] = {120, 120, 180, 120, 120, 120};
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

        ConsoleGame::AniSprite itemSpeed;
        ConsoleGame::AniSprite itemStar;
        ConsoleGame::AniSprite itemHealth;

        MobSprite mobSpriteEasy;
        MobSprite mobSpriteNormal;
        MobSprite mobSpriteHard;

        ConsoleGame::AniSprite deathVfx;
    };

    struct GameMapData {
        MapType mapType;
        CharaType charaType;
        MapMode mapMode;
        MapDifficulty mapDifficulty;
        bool enableDebuff;
    };

    struct GameAudio {
        ConsoleGame::Audio damageSfx;
        ConsoleGame::Audio deadSfx;
        ConsoleGame::Audio warningSfx;
        ConsoleGame::Audio scoreSfx;

        ConsoleGame::Audio debuffActivateSfx;
        ConsoleGame::Audio debuffOverSfx;
        ConsoleGame::Audio shieldBreakSfx;

        ConsoleGame::Audio skillFaunaSfx;
        ConsoleGame::Audio skillIrysSfx;
        ConsoleGame::Audio skillMumeiSfx;
        ConsoleGame::Audio skillKroniiSfx;
        ConsoleGame::Audio skillSanaSfx;
        ConsoleGame::Audio skillBaeSfx;
        ConsoleGame::Audio skillReadySfx;
        ConsoleGame::Audio skillOverSfx;

        ConsoleGame::Audio itemPickSfx;
    };

    struct UserOption {
        uint8_t character : 3;
        uint8_t map : 3;
        uint8_t debuff : 1;
        uint8_t isTimed : 1;
        uint8_t music : 3;

        uint16_t time;
    };

    struct GameResult {
        size_t score = 0;
        size_t time = 0;
        size_t damage = 0;
        size_t numOfMob = 0;
        size_t numOfSkill = 0;
        size_t numOfItem = 0;
        MobType diff;
        MapType map;
    };

}  // namespace GameType