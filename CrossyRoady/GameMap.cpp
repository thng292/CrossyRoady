#include "GameMap.h"

using namespace ConsoleGame;
using namespace GameUtils;
using namespace GameType;

const std::wstring_view GameMap::ScreenName() { return L"GameMap"; }

std::wstring_view GameMap::getName() { return ScreenName(); }

void GameMap::Init(const std::any& args)
{
    gameEventArgs.mapDebuffCooldownTime = MAP_DEBUFF_COOLDOWN;
    gameEventArgs.mapDebuffTime = MAP_DEBUFF_DURATION;
    gameEventArgs.originalHealth = 0;

    gameFlags.isDebuff = false;
    gameFlags.debuffCalled = false;
    gameFlags.allowCharacterKeys = true;
    gameFlags.isReverseKey = false;
}

AbstractScreen* GameMap::Clone() const { return new GameMap; }

AbstractNavigation::NavigationRes GameMap::Update(
    float deltaTime, const AbstractNavigation* navigation
)
{
    ResetFlags();
    DragMapDown(deltaTime);

    UpdateLanes(deltaTime);
    UpdateCooldowns(deltaTime);

    CollisionCheck();
    DebuffCheck();

    HandleDebuff(deltaTime);
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
    gm.charaType = IRYS;
    gm.mapMode = INF;
    gm.mapType = CASINO;

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
    LoadMapSprite(gameData.mapType, gameSprites.debuff, "debuff");

    LoadExtraSprite(gameSprites.emptyHealth, "health-empty");
    LoadCharaSprite(gameData.charaType, gameSprites.health, "health");
    LoadCharaSprite(gameData.charaType, gameSprites.skill, "skill");

    ChangeColorPalette(GetGamePalette(gameData.mapType, gameData.charaType));

    AniSprite cur = gameSprites.mobSpriteEasy.MobRight;
    MobType ty = EASY;

    for (int i = 0; i < 10; ++i) {
        laneList.push_back(std::make_unique<Road>(
            32 * (i + 1), ty, gameSprites.roadSprite, cur
        ));
        /* laneList.push_back(std::make_unique<Water>(
             32 * (i + 1),
             gameSprites.roadSprite,
             gameSprites.floatSprite
         ));*/
        /* laneList.push_back(std::make_unique<SafeZone>(
             32 * (i + 1),
             gameSprites.roadSprite,
             gameSprites.blockSprite
         ));*/
    }
}

void GameMap::Unmount() {}

void GameMap::HandlePlayerInput()
{
    if (gameFlags.allowCharacterKeys) {
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
    float distance = (character.getSpeed()) * deltaTime;
    bool moveFlag = true;
    if (gameFlags.movingLeft && gameFlags.allowMoveLeft) {
        character.MoveLeft(distance);
    } else if (gameFlags.movingRight && gameFlags.allowMoveRight) {
        character.MoveRight(distance);
    } else if (gameFlags.movingUp && gameFlags.allowMoveUp) {
        character.MoveUp(distance);
    } else if (gameFlags.movingDown && gameFlags.allowMoveDown) {
        character.MoveDown(distance);
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
    if (gameFlags.isDebuff) DrawDebuff(canvas);
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

    if (gameFlags.isDebuff == false) {
        gameFlags.isFaunaDebuff = false;
        gameFlags.isIrysDebuff = false;
        gameFlags.isMumeiDebuff = false;
        gameFlags.isKroniiDebuff = false;
        gameFlags.isBaeDebuff = false;
        gameFlags.isSanaDebuff = false;
    }
}

void GameMap::CollisionCheck()
{
    for (auto& lane : laneList) {
        if (lane->GetType() == LaneType::WATER) {
            CollisionType waterColType = lane->GetLaneCollision(character);
            if (waterColType != CollisionType::None) {
                HandleWaterCollision(waterColType);
            }
        }
        CollisionType colType = lane->GetCollision(character);
        if (colType != CollisionType::None) {
            HandleCollision(lane, colType);
        }
    }
}

void GameMap::DebuffCheck()
{
    if (!gameFlags.isDebuff) return;
    if (gameFlags.debuffCalled) return;
    MapType mapType = gameData.mapType;
    if (mapType == CASINO) {
        int randInd = std::rand() % (CASINO + 1);
        mapType = static_cast<MapType>(randInd);
    }
    LogDebug("{}", static_cast<int>(mapType));
    switch (mapType) {
        case FOREST:
            gameFlags.isFaunaDebuff = true;
            break;
        case CITY:
            gameFlags.isIrysDebuff = true;
            break;
        case HOUSE:
            gameFlags.isMumeiDebuff = true;
            break;
        case DESERT:
            gameFlags.isKroniiDebuff = true;
            break;
        case SPACE:
            gameFlags.isSanaDebuff = true;
            break;
        case CASINO:
            gameFlags.isBaeDebuff = true;
            break;
    }
    gameFlags.debuffCalled = true;
}

void GameMap::UpdateLanes(float deltaTime)
{
    for (auto& lane : laneList) {
        LaneType type = lane->GetType();
        if (type == LaneType::SAFE) {
            continue;
        }
        lane->UpdatePos(deltaTime);
        if (type == LaneType::ROAD) {
            auto castedLane = dynamic_cast<Road*>(lane.get());
            // castedLane->UpdateSprite(deltaTime);
        }
    }
}

void GameMap::HandleCollision(
    const std::unique_ptr<Lane>& lane, CollisionType colType
)
{
    int newHealth;
    Road* road = nullptr;
    switch (lane->GetType()) {
        case ROAD:
            road = dynamic_cast<Road*>(lane.get());
            gameEventArgs.collidedMobtype = road->GetMobType();
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

void GameMap::HandleDamage()
{
    if (gameFlags.isDamageCooldown || !gameFlags.damageCollision) return;
    int newHealth =
        character.GetCurHealth() - (gameEventArgs.collidedMobtype + 1);
    character.SetCurHealth(newHealth);
    gameFlags.isDamageCooldown = true;
    gameEventArgs.damageCooldownTime = 3;
}

void GameMap::HandleDebuff(float deltaTime)
{
    if (!gameFlags.isDebuff) return;

    if (gameFlags.isFaunaDebuff) {
        if (gameFlags.isMoving == false) {
            gameEventArgs.notMovingTime += deltaTime;
        }
        if (gameEventArgs.notMovingTime >= MAX_IDLE_TIME) {
            int newHealth = character.GetCurHealth() - 1;
            character.SetCurHealth(newHealth);
            gameEventArgs.notMovingTime = 0;
        }
    }

    else if (gameFlags.isIrysDebuff) {
        int curHealth = character.GetCurHealth();
        if (curHealth > IRYS_DEBUFF_HEALTH) {
            gameEventArgs.originalHealth = curHealth;
            character.SetCurHealth(IRYS_DEBUFF_HEALTH);
        }
    } else if (gameFlags.isMumeiDebuff) {
        gameFlags.isDarkMap = true;
    } else if (gameFlags.isKroniiDebuff) {
        gameFlags.allowCharacterKeys = false;
        // gameFlags.allowSkill = false;
    } else if (gameFlags.isSanaDebuff) {
        gameFlags.allowSkill = false;
        gameFlags.allowDebuffSkill = false;
    } else if (gameFlags.isBaeDebuff) {
        gameFlags.isReverseKey = true;
    }

    gameEventArgs.mapDebuffTime -= deltaTime;
    if (gameEventArgs.mapDebuffTime <= 0) {
        gameFlags.isDebuff = false;
        gameFlags.debuffCalled = false;
        gameEventArgs.mapDebuffTime = MAP_DEBUFF_DURATION;
        gameEventArgs.mapDebuffCooldownTime = MAP_DEBUFF_COOLDOWN;

        if (gameEventArgs.originalHealth != 0) {
            character.SetCurHealth(gameEventArgs.originalHealth);
        }

        gameEventArgs.notMovingTime = 0;
        gameEventArgs.originalHealth = 0;
        gameFlags.allowCharacterKeys = true;
        gameFlags.allowSkill = true;
        gameFlags.allowDebuffSkill = true;
        gameFlags.isDarkMap = false;
        gameFlags.isReverseKey = false;
    }
}

void GameMap::UpdateCooldowns(float deltaTime)
{
    if (gameFlags.isDamageCooldown) {
        gameEventArgs.damageCooldownTime -= deltaTime;
    }

    if (!gameFlags.isDebuff) {
        gameEventArgs.mapDebuffCooldownTime -= deltaTime;
    }

    if (gameEventArgs.damageCooldownTime <= 0) {
        gameFlags.isDamageCooldown = false;
    }

    if (gameEventArgs.mapDebuffCooldownTime <= 0) {
        gameFlags.isDebuff = true;
    }
}

void GameMap::DragMapDown(float deltatime)
{
    auto laneListEnd = laneList.end();
    for (auto it = laneList.begin(); it != laneListEnd; ++it) {
        auto lane = it->get();
        lane->SetY(lane->GetY() - deltatime * mapSpeed);
        lane->GetY();
    }

    if (!laneList.empty()) {
        float roadTopY = laneList.front()->GetTopY();
        if (roadTopY < 0) {
            laneList.erase(laneList.begin());
            int tmp = rand() % 2;
            switch (tmp) {
                case 0:
                    laneList.push_back(std::make_unique<Road>(
                        laneList.back()->GetY() + 32,
                        MobType::NORMAL,
                        gameSprites.roadSprite,
                        gameSprites.mobSpriteNormal.MobRight
                    ));
                    break;
                case 1:
                    laneList.push_back(std::make_unique<Water>(
                        laneList.back()->GetY() + 32,
                        gameSprites.roadSprite,
                        gameSprites.floatSprite
                    ));
                    break;
            }
        }
    }
}
