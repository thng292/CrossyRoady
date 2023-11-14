#include "GameMap.h"

using namespace ConsoleGame;
using namespace GameUtils;
using namespace GameType;

void GameMap::InitLaneList()
{
    int initSafeCount = 3;
    for (size_t i = 0; i < initSafeCount; ++i) {
        laneList.push_back(std::make_unique<SafeZone>(
            32 * (i + 1),
            gameSprites.safeSprite,
            gameSprites.blockSprite,
            true,
            true
        ));
    }

    int screenHeight = _CONSOLE_HEIGHT_ * 2;
    int maxLane = screenHeight / 32;
    for (size_t i = 0; i < maxLane; ++i) {
        laneList.push_back(GetRandomLane());
    }
}

const std::wstring_view GameMap::ScreenName() { return L"GameMap"; }

std::wstring_view GameMap::getName() { return ScreenName(); }

void GameMap::Init(const std::any& args) {}

AbstractScreen* GameMap::Clone() const { return new GameMap; }

AbstractNavigation::NavigationRes GameMap::Update(
    float deltaTime, const AbstractNavigation* navigation
)
{
    if (character.GetY() >= _CONSOLE_HEIGHT_) {
        mapSpeedY = character.getSpeed();
    } else {
        mapSpeedY = MAP_SPEED;
    }
    ResetFlags();
    DragMapDown(deltaTime);

    UpdateLanes(deltaTime);
    UpdateCooldowns(deltaTime);

    CollisionCheck(deltaTime);
    DebuffCheck();
    SkillCheck();

    HandleDebuff(deltaTime);
    HandleSkill(deltaTime);
    HandleDamage();
    HandlePlayerInput();
    HandlePlayerMovement(deltaTime);
    HandlePlayerAnimation(deltaTime);
    return navigation->NoChange();
}

void GameMap::Mount(const std::any& args)
{
    /*const GameType::GameMapData& gameDataArg =
        std::any_cast<const GameMapData&>(args);*/

    GameMapData gm;
    gm.charaType = MUMEI;
    gm.mapMode = INF;
    gm.mapType = FOREST;

    SetGameMapData(gm);

    character.Init(gameData.charaType);

    // mob sprites
    LoadMobSprite(gameData.mapType, MobType::EASY, gameSprites.mobSpriteEasy);
    LoadMobSprite(
        gameData.mapType, MobType::NORMAL, gameSprites.mobSpriteNormal
    );
    LoadMobSprite(gameData.mapType, MobType::HARD, gameSprites.mobSpriteHard);

    // static sprites
    LoadMapSprite(gameData.mapType, gameSprites.blockSprite, "block");
    LoadMapSprite(gameData.mapType, gameSprites.floatSprite, "float");
    LoadMapSprite(gameData.mapType, gameSprites.roadSprite, "road");
    LoadMapSprite(gameData.mapType, gameSprites.safeSprite, "safe");
    LoadMapSprite(gameData.mapType, gameSprites.waterSprite, "water");
    LoadMapSprite(gameData.mapType, gameSprites.debuff, "debuff");

    LoadExtraSprite(gameSprites.emptyHealth, "health-empty");
    LoadCharaSprite(gameData.charaType, gameSprites.health, "health");
    LoadCharaSprite(gameData.charaType, gameSprites.skill, "skill");

    ResiseBlockHitBox();
    ChangeColorPalette(GetGamePalette(gameData.mapType, gameData.charaType));
    InitLaneList();
}

void GameMap::Unmount() {}

void GameMap::HandlePlayerInput()
{
    if (gameFlags.allowMovementKeys) {
        bool correctKeyFlag = !gameFlags.isReverseKey;
        if (IsKeyMeanUp()) {
            gameFlags.movingUp = correctKeyFlag;
            gameFlags.movingDown = !correctKeyFlag;
        } else if (IsKeyMeanDown()) {
            gameFlags.movingDown = correctKeyFlag;
            gameFlags.movingUp = !correctKeyFlag;
        } else if (IsKeyMeanLeft()) {
            gameFlags.movingLeft = correctKeyFlag;
            gameFlags.movingRight = !correctKeyFlag;
        } else if (IsKeyMeanRight()) {
            gameFlags.movingRight = correctKeyFlag;
            gameFlags.movingLeft = !correctKeyFlag;
        }
    }
    if (IsKeyMeanSelect()) {
        gameFlags.skillCalled = true;
    }
}

void GameMap::HandlePlayerAnimation(float deltaTime)
{
    if (gameFlags.movingLeft) {
        character.SetSpriteLeft();
    } else if (gameFlags.movingRight) {
        character.SetSpriteRight();
    } else if (gameFlags.movingUp) {
        character.SetSpriteUp();
    } else if (gameFlags.movingDown) {
        character.SetSpriteDown();
    }

    if (gameFlags.isMoving) {
        if (gameFlags.justMoved) {
            character.AdvanceFrame();
        }
        character.UpdateFrame(deltaTime);

    } else {
        character.ResetSprite();
    }
}

void GameMap::HandlePlayerMovement(float deltaTime)
{
    bool moveFlag = true;
    float distanceY = character.getSpeed() * deltaTime;
    if (gameFlags.movingLeft && gameFlags.allowMoveLeft) {
        float distanceX;
        if (mapSpeedX < 0) {
            distanceX = deltaTime * (character.getSpeed() - mapSpeedX);
        } else {
            distanceX = deltaTime * (character.getSpeed() + mapSpeedX);
        }
        character.MoveLeft(distanceX);
    } else if (gameFlags.movingRight && gameFlags.allowMoveRight) {
        float distanceX;
        if (mapSpeedX > 0) {
            distanceX = deltaTime * (character.getSpeed() + mapSpeedX);
        } else {
            distanceX = deltaTime * (character.getSpeed() - mapSpeedX);
        }
        character.MoveRight(distanceX);
    } else if (gameFlags.movingUp && gameFlags.allowMoveUp) {
        character.MoveUp(distanceY);
        tempScore += distanceY;
        if (tempScore > currentScore) {
            currentScore = tempScore;
        }
    } else if (gameFlags.movingDown && gameFlags.allowMoveDown) {
        character.MoveDown(distanceY);
        tempScore -= distanceY;
    } else {
        moveFlag = false;
    }

    if (moveFlag) {
        if (gameFlags.isMoving == false) {
            gameFlags.justMoved = true;
        } else {
            gameFlags.justMoved = false;
        }
        gameFlags.isMoving = true;

    } else {
        gameFlags.isMoving = false;
    }
}

void GameMap::TurnOffDebuff()
{
    gameFlags.debuffInUse = false;
    gameEventArgs.mapDebuffTime = DEBUFF_DURATION[gameEventArgs.debuffType];
    gameEventArgs.mapDebuffCooldownTime = MAP_DEBUFF_COOLDOWN;

    switch (gameEventArgs.debuffType) {
        case FOREST:
            gameEventArgs.notMovingTime = 0;
            break;
        case CITY:
            character.SetCurHealth(gameEventArgs.originalHealth);
            gameEventArgs.originalHealth = 0;
            break;
        case HOUSE:
            gameFlags.isDarkMap = false;
            break;
        case DESERT:
            gameFlags.allowMovementKeys = true;
            gameFlags.allowSkillKey = true;
            break;
        case SPACE:
            gameFlags.allowSkill = true;
            break;
        case CASINO:
            gameFlags.isReverseKey = false;
            break;
    }
}

void GameMap::TurnOffSkill()
{
    switch (gameEventArgs.skillType) {
        case FAUNA:
            character.SetMaxHealth(FAUNA_MAX_HEALTH);
            if (character.GetCurHealth() >= FAUNA_MAX_HEALTH) {
                character.SetCurHealth(FAUNA_MAX_HEALTH);
            }
            break;
        case IRYS:
            gameEventArgs.shield = 0;
            break;
        case MUMEI:
            character.SetSpeed(gameEventArgs.originalSpeed);
            gameFlags.isInvincible = false;
            break;
        case KRONII:
            gameFlags.allowLaneUpdate = true;
            break;
        case SANA:
            gameFlags.allowDebuff = true;
            break;
        case BAE:
            gameFlags.isReverseKey = false;
            break;
    }

    gameFlags.turnOffSkill = false;
    gameFlags.skillInUse = false;
    gameFlags.allowSkill = true;
}

std::unique_ptr<Lane> GameMap::GetRandomLane()
{
    int randInd = rand() % 4;
    bool isLeftToRight = rand() % 2;
    AniSprite mobSprite = GetMobSprite(isLeftToRight);
    switch (randInd) {
        case 0:
            return std::make_unique<Road>(
                laneList.back()->GetY() + 32,
                currentDifficulty,
                gameSprites.roadSprite,
                mobSprite,
                isLeftToRight
            );
            break;
        case 1:
            return std::make_unique<Rail>(
                laneList.back()->GetY() + 32,
                currentDifficulty,
                gameSprites.roadSprite,
                mobSprite,
                isLeftToRight
            );
            break;
        case 2:
            return std::make_unique<SafeZone>(
                laneList.back()->GetY() + 32,
                gameSprites.safeSprite,
                gameSprites.blockSprite,
                isLeftToRight
            );
            break;
        case 3:
            return std::make_unique<Water>(
                laneList.back()->GetY() + 32,
                gameSprites.waterSprite,
                gameSprites.floatSprite,
                isLeftToRight
            );
            break;
    }
}

ConsoleGame::AniSprite GameMap::GetMobSprite(bool isLeftToRight)
{
    if (isLeftToRight) {
        switch (currentDifficulty) {
            case EASY:
                return gameSprites.mobSpriteEasy.MobRight;
                break;
            case NORMAL:
                return gameSprites.mobSpriteNormal.MobRight;
                break;
            case HARD:
                return gameSprites.mobSpriteHard.MobRight;
                break;
        }
    } else {
        switch (currentDifficulty) {
            case EASY:
                return gameSprites.mobSpriteEasy.MobLeft;
                break;
            case NORMAL:
                return gameSprites.mobSpriteNormal.MobLeft;
                break;
            case HARD:
                return gameSprites.mobSpriteHard.MobLeft;
                break;
        }
    }
}

void GameMap::ResiseBlockHitBox()
{
    Box hitbox = gameSprites.blockSprite.GetHitBox();
    if (hitbox.dim.height <= 32) return;
    int yOffset = hitbox.dim.height - 32;
    int newHeight = 32;
    Vec2 coordOffSet = {.x = 0, .y = yOffset};
    Vec2 dimOffset = {.width = 0, .height = -(hitbox.dim.height - 32)};
    gameSprites.blockSprite.EditHitBox(coordOffSet, dimOffset);
};

void GameMap::SetGameMapData(const GameMapData& gmData)
{
    gameData.mapType = gmData.mapType;
    gameData.charaType = gmData.charaType;
    gameData.mapMode = gmData.mapMode;
}

void GameMap::Draw(AbstractCanvas* canvas) const
{
    DrawFlat(canvas);
    DrawEntity(canvas);
    DrawHealth(canvas);
    DrawSkill(canvas);
    if (gameFlags.debuffInUse) DrawDebuff(canvas);
    if (gameFlags.isDarkMap) DrawDarkness(canvas);
}

void GameMap::DrawFlat(ConsoleGame::AbstractCanvas* canvas) const
{
    auto laneListEnd = laneList.end();
    int screenHeight = _CONSOLE_HEIGHT_ * 2 + 32;
    for (auto it = laneList.begin(); it != laneListEnd; ++it) {
        Lane* lane = it->get();
        if (lane->GetY() <= screenHeight) {
            lane->DrawLane(canvas);
        }
    }
}

void GameMap::DrawEntity(ConsoleGame::AbstractCanvas* canvas) const
{
    auto laneListEnd = laneList.rend();
    bool charaDrawn = false;
    float charBottomY = character.GetBottomY();
    int screenHeight = _CONSOLE_HEIGHT_ * 2 + 32;

    for (auto it = laneList.rbegin(); it != laneListEnd; ++it) {
        Lane* lane = it->get();
        if (lane->GetY() <= screenHeight) {
            if (!charaDrawn && lane->GetType() != WATER) {
                if (charBottomY > lane->GetBottomY()) {
                    character.Draw(canvas);
                    charaDrawn = true;
                }
            }
            lane->DrawEntity(canvas);
        }
    }
    if (!charaDrawn) {
        character.Draw(canvas);
    }
}

void GameMap::DrawHealth(AbstractCanvas* canvas) const
{
    Vec2 coord{.x = 5, .y = 4};
    int maxHealth = character.getMaxHealth();
    int curHealth = character.GetCurHealth();
    int margin = 0;
    int space = 9 + margin;
    for (int i = 0; i < maxHealth; ++i) {
        gameSprites.emptyHealth.Paint(canvas, coord);
        coord.x += space;
    }
    coord.x = 5;
    for (int i = 0; i < curHealth; ++i) {
        gameSprites.health.Paint(canvas, coord);
        coord.x += space;
    }
}

void GameMap::DrawSkill(ConsoleGame::AbstractCanvas* canvas) const
{
    Vec2 coord{.x = 3, .y = 12};
    gameSprites.skill.Paint(canvas, coord);
}

void GameMap::DrawDebuff(ConsoleGame::AbstractCanvas* canvas) const
{
    Vec2 coord{.x = ConsoleGame::_CONSOLE_WIDTH_ - 21, .y = 4};
    gameSprites.debuff.Paint(canvas, coord);
}

void GameMap::DrawDarkness(ConsoleGame::AbstractCanvas* canvas) const
{
    Box charaBox = character.GetHitBox();
    int screenWidth = _CONSOLE_WIDTH_;
    int screenHeight = _CONSOLE_HEIGHT_ * 2;
    int xCenter = charaBox.coord.x + charaBox.dim.width / 2;
    int yCenter = screenHeight - charaBox.coord.y;

    int visibleRadius = VISIBLE_RADIUS;
    Color darknessColor = Color::BLACK;
    for (int y = 0; y <= screenHeight; ++y) {
        for (int x = 0; x <= screenWidth; ++x) {
            if (GetDistance(xCenter, yCenter, x, y) > visibleRadius) {
                (*canvas)[y][x] = darknessColor;
            }
        }
    }
}

void GameMap::ResetFlags()
{
    gameFlags.allowMoveRight = true;
    gameFlags.allowMoveLeft = true;
    gameFlags.allowMoveUp = true;
    gameFlags.allowMoveDown = true;

    gameFlags.movingLeft = false;
    gameFlags.movingRight = false;
    gameFlags.movingUp = false;
    gameFlags.movingDown = false;

    gameFlags.blockCollision = false;
    gameFlags.damageCollision = false;
    gameFlags.logCollision = false;
    gameFlags.itemCollision = false;
}

void GameMap::CollisionCheck(float deltaTime)
{
    for (auto& lane : laneList) {
        if (lane->GetType() == LaneType::WATER) {
            CollisionType waterColType = lane->GetLaneCollision(character);
            if (waterColType != CollisionType::Bottom &&
                waterColType != CollisionType::Top) {
                if (lane->ContainsChara(character)) {
                    HandleCharaOnLog(lane, deltaTime);
                }
            }

            HandleWaterCollision(waterColType);

        } else {
            CollisionType colType = lane->GetCollision(character);
            if (colType != CollisionType::None) {
                HandleCollision(lane, colType);
            }
        }
    }
}

void GameMap::DebuffCheck()
{
    if (!gameFlags.debuffCalled) return;
    gameFlags.debuffCalled = false;

    MapType mapType = gameData.mapType;
    if (mapType == CASINO) {
        int randInd = std::rand() % (CASINO + 1);
        mapType = static_cast<MapType>(randInd);
    }
    gameEventArgs.debuffType = mapType;
    gameFlags.debuffInUse = true;
}

void GameMap::SkillCheck()
{
    if (!gameFlags.skillCalled) return;
    gameFlags.skillCalled = false;
    if (!gameFlags.allowSkill) return;

    CharaType charaType = gameData.charaType;
    if (charaType == BAE) {
        int randInd = std::rand() % (BAE + 1);
        charaType = static_cast<CharaType>(randInd);
    }
    gameEventArgs.skillType = charaType;
    gameFlags.skillActivate = true;
}

void GameMap::UpdateLanes(float deltaTime)
{
    if (!gameFlags.allowLaneUpdate) return;
    for (auto& lane : laneList) {
        LaneType type = lane->GetType();
        if (type == LaneType::SAFE) {
            continue;
        }
        lane->UpdatePos(deltaTime);
        if (type == LaneType::ROAD) {
            auto castedLane = dynamic_cast<Road*>(lane.get());
            castedLane->UpdateSprite(deltaTime);
        }
        if (type == LaneType::RAIL) {
            auto castedLane = dynamic_cast<Rail*>(lane.get());
            castedLane->UpdateSprite(deltaTime);
        }
    }
}

void GameMap::HandleCollision(
    const std::unique_ptr<Lane>& lane, CollisionType colType
)
{
    int newHealth;
    Road* road = nullptr;
    Rail* rail = nullptr;
    switch (lane->GetType()) {
        case ROAD:
            road = dynamic_cast<Road*>(lane.get());
            gameEventArgs.collidedMobtype = road->GetMobType();
            gameFlags.damageCollision = true;
            break;
        case RAIL:
            rail = dynamic_cast<Rail*>(lane.get());
            gameEventArgs.collidedMobtype = rail->GetMobType();
            gameFlags.damageCollision = true;
            break;
        case SAFE:
            gameFlags.blockCollision = true;
            if (colType == CollisionType::Left) {
                gameFlags.allowMoveRight = false;
            } else if (colType == CollisionType::Right) {
                gameFlags.allowMoveLeft = false;
            } else if (colType == CollisionType::Top) {
                gameFlags.allowMoveDown = false;
            } else if (colType == CollisionType::Bottom) {
                gameFlags.allowMoveUp = false;
            }
            break;
        case WATER:
            break;
    }
}

void GameMap::HandleWaterCollision(GameType::CollisionType colType)
{
    if (colType == CollisionType::Left) {
        gameFlags.allowMoveRight = false;
    } else if (colType == CollisionType::Right) {
        gameFlags.allowMoveLeft = false;
    } else if (colType == CollisionType::Top) {
        gameFlags.allowMoveDown = false;
    } else if (colType == CollisionType::Bottom) {
        gameFlags.allowMoveUp = false;
    }
}

void GameMap::HandleCharaOnLog(
    const std::unique_ptr<Lane>& lane, float deltaTime
)
{
    float laneSpeed = lane->GetSpeed();

    float distance = deltaTime * laneSpeed;
    if (lane->GetIsLeftToRight()) {
        character.MoveRight(distance);
        mapSpeedX = laneSpeed;
    } else {
        character.MoveLeft(distance);
        mapSpeedX = -laneSpeed;
    }
}

void GameMap::HandleDamage()
{
    if (gameFlags.isDamageCooldown || !gameFlags.damageCollision ||
        gameFlags.isInvincible)
        return;
    if (gameEventArgs.shield > 0) {
        --gameEventArgs.shield;
    } else {
        int newHealth =
            character.GetCurHealth() - (gameEventArgs.collidedMobtype + 1);
        character.SetCurHealth(newHealth);
    }
    gameFlags.isDamageCooldown = true;
    gameEventArgs.damageCooldownTime = 3;
}

void GameMap::HandleDebuff(float deltaTime)
{
    if (!gameFlags.debuffInUse) return;
    int curHealth = character.GetCurHealth();
    switch (gameEventArgs.debuffType) {
        case FOREST:
            if (gameFlags.isMoving == false) {
                gameEventArgs.notMovingTime += deltaTime;
            }
            if (gameEventArgs.notMovingTime >= MAX_IDLE_TIME) {
                int newHealth = character.GetCurHealth() - 1;
                character.SetCurHealth(newHealth);
                gameEventArgs.notMovingTime = 0;
            }
            break;
        case CITY:
            if (curHealth > IRYS_DEBUFF_HEALTH) {
                gameEventArgs.originalHealth = curHealth;
                character.SetCurHealth(IRYS_DEBUFF_HEALTH);
            }
            break;
        case HOUSE:
            gameFlags.isDarkMap = true;
            break;
        case DESERT:
            gameFlags.allowMovementKeys = false;
            if (gameData.charaType != SANA) {
                gameFlags.allowSkillKey = false;
            }
            break;
        case SPACE:
            gameFlags.allowSkill = false;
            TurnOffSkill();
            break;
        case CASINO:
            gameFlags.isReverseKey = true;
            break;
    }

    gameEventArgs.mapDebuffTime -= deltaTime;
    if (gameEventArgs.mapDebuffTime <= 0) {
        TurnOffDebuff();
    }
}

void GameMap::HandleSkill(float deltaTime)
{
    if (gameFlags.skillActivate) {
        gameEventArgs.skillTime = SKILL_DURATION;
        int curSpeed = character.getSpeed();
        switch (gameEventArgs.skillType) {
            case FAUNA:
                character.SetMaxHealth(FAUNA_EXTRA_MAX_HEALTH);
                character.SetCurHealth(FAUNA_EXTRA_MAX_HEALTH);
                gameEventArgs.skillCategory = TIME;
                break;
            case IRYS:
                gameEventArgs.shield = IRYS_SHIELD_COUNT;
                gameEventArgs.skillCategory = SHIELD;
                break;
            case MUMEI:
                gameEventArgs.originalSpeed = curSpeed;
                character.SetSpeed(curSpeed + MUMEI_SPEED_BUFF);
                gameFlags.isInvincible = true;
                gameEventArgs.skillCategory = TIME;
                break;
            case KRONII:
                gameFlags.allowLaneUpdate = false;
                gameEventArgs.skillCategory = TIME;
                break;
            case SANA:
                TurnOffDebuff();
                gameEventArgs.skillCategory = TIME;
                break;
            case BAE:
                gameFlags.isReverseKey = true;
                gameEventArgs.skillCategory = TIME;
                break;
        }
        gameFlags.skillActivate = false;
        gameFlags.skillInUse = true;
        gameFlags.allowSkill = false;
    }
    if (gameFlags.skillInUse) {
        switch (gameEventArgs.skillCategory) {
            case TIME:
                gameEventArgs.skillTime -= deltaTime;
                if (gameEventArgs.skillTime <= 0) {
                    gameFlags.turnOffSkill = true;
                }
                break;
            case SHIELD:
                if (gameEventArgs.shield == 0) {
                    gameFlags.turnOffSkill = true;
                }
                break;
        }
    }

    if (gameFlags.turnOffSkill) {
        TurnOffSkill();
    }
}

void GameMap::UpdateCooldowns(float deltaTime)
{
    if (gameFlags.isDamageCooldown) {
        gameEventArgs.damageCooldownTime -= deltaTime;
    }

    if (!gameFlags.debuffInUse) {
        gameEventArgs.mapDebuffCooldownTime -= deltaTime;
    }

    if (gameEventArgs.damageCooldownTime <= 0) {
        gameFlags.isDamageCooldown = false;
    }

    if (gameEventArgs.mapDebuffCooldownTime <= 0) {
        gameFlags.debuffCalled = true;
    }
}

void GameMap::DragMapDown(float deltatime)
{
    float distance = deltatime * mapSpeedY;
    character.MoveDown(distance);

    auto laneListEnd = laneList.end();
    for (auto it = laneList.begin(); it != laneListEnd; ++it) {
        auto lane = it->get();
        lane->SetY(lane->GetY() - distance);
    }

    if (!laneList.empty()) {
        float roadTopY = laneList.front()->GetTopY();
        if (roadTopY < 0) {
            laneList.erase(laneList.begin());
            laneList.push_back(GetRandomLane());
        }
    }
}
