#pragma once
#include "GameType.h"
#include "Lane.h"

namespace GameMaster {
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
        bool allowMapDrag : 1 = true;

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
        bool debuffWarning : 1 = false;

        bool skillCalled : 1 = false;
        bool skillActivate : 1 = false;
        bool skillInUse : 1 = false;
        bool turnOffSkill : 1 = false;

        bool isInvincible : 1 = false;

        bool mapHasItem : 1 = false;

        // Action
        bool isDarkMap : 1 = false;
        bool isReverseKey : 1 = false;
        bool isGameOver : 1 = false;
        bool isOnLog : 1 = false;

        // Pause
        bool gamePaused : 1 = false;
        bool isFirstMount : 1 = true;
        bool savedGame : 1 = true;

        bool isFaunaSkill : 1 = false;
        bool isIrysSkill : 1 = false;
        bool isMumeiSkill : 1 = false;
        bool isKroniiSkill : 1 = false;
        bool isSanaSkill : 1 = false;
        bool isBaeSkill : 1 = false;
    };

    struct GameEventsArgs {
        GameType::MobType collidedMobtype = GameType::EASY;
        GameType::MapType debuffType = GameType::FOREST;
        GameType::CharaType skillType = GameType::FAUNA;
        GameType::SkillCategory skillCategory = GameType::TIME;
        float gameOverWait = 3.0f;

        float visibleRadius = GameType::MAX_VISIBLE_RADIUS;
        float minVisibleRadius;

        // Map stuff
        float mapSpeedY = 0;
        float mapSpeedX = 0;
        float timeLeft = 0;

        // Results
        size_t currentScore = 0;
        size_t damageTaken = 0;
        size_t numOfMobsHit = 0;
        size_t numOfSkillUse = 0;
        size_t numOfItemPick = 0;
        GameType::MobType difficultyReached;
        float playTime = 0;

        // Cooldown
        float damageCooldownTime = GameType::DAMAGE_COOLDOWN;
        float mapDebuffCooldownTime = GameType::MAP_DEBUFF_COOLDOWN;

        // Walking
        float distWalkedScore = 0;
        float distWalkedSkill = 0;
        float distWalked = 0;

        // Difficulty
        size_t mobRange;
        float mapDragSpeed = 0;

        // Animation
        float damageFlashingTimer = 0;
        float debuffFlasingTimer = 0;

        // Duration
        float mapDebuffTime;
        float skillTime = GameType::SKILL_DURATION;

        // Miscel
        float notMovingTime = 0;
        float originalHealth = 0;
        float originalSpeed = 0;

        // Skill
        int skillCharge = 0;
        int shield = 0;
        uint8_t skillStep = 1;
    };
}  // namespace GameMaster