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
    };

    struct GameEventsArgs {
        GameType::MobType collidedMobtype;
        GameType::MapType debuffType;
        GameType::CharaType skillType;
        GameType::SkillCategory skillCategory;

        // Cooldown
        float damageCooldownTime = GameType::DAMAGE_COOLDOWN;
        float mapDebuffCooldownTime = GameType::MAP_DEBUFF_COOLDOWN;

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
        float skillCharge = 0;
        int shield = 0;

        // Item
        Lane* laneWithItem;
    };
}  // namespace GameMaster