#include "GameMap.h"

using namespace ConsoleGame;
using namespace GameUtils;
using namespace GameType;

const std::wstring_view GameMap::ScreenName() { return L"GameMap"; }

void GameMap::InitRoadPosList() {}

void GameMap::AddRoad() {}

void GameMap::DeleteRoad() {}

std::wstring_view GameMap::getName() { return ScreenName(); }

void GameMap::Init(const std::any& args) {}

AbstractScreen* GameMap::Clone() const { return new GameMap; }

AbstractNavigation::NavigationRes GameMap::Update(
    float deltaTime, const AbstractNavigation* navigation
)
{
    ResetFlags();
    DragMapDown(deltaTime);
    CollisionCheck();
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
    LoadMapSprite(gameData.mapType, gameSprites.debuff, "debuff");

    LoadExtraSprite(gameSprites.emptyHealth, "health-empty");
    LoadCharaSprite(gameData.charaType, gameSprites.health, "health");
    LoadCharaSprite(gameData.charaType, gameSprites.skill, "skill");

    ChangeColorPalette(GetGamePalette(gameData.mapType, gameData.charaType));

    AniSprite cur = gameSprites.mobSpriteHard.MobRight;
    MobType ty = HARD;

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
    if (IsKeyMeanUp()) {
        gameFlags.movingUp = true;
    } else if (IsKeyMeanDown()) {
        gameFlags.movingDown = true;
    } else if (IsKeyMeanLeft()) {
        gameFlags.movingLeft = true;
    } else if (IsKeyMeanRight()) {
        gameFlags.movingRight = true;
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
    DrawDebuff(canvas);
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
    size_t maxHealth = character.getMaxHealth();
    size_t curHealth = character.GetCurHealth();
    size_t margin = 0;
    size_t space = 9 + margin;
    for (size_t i = 0; i < maxHealth; ++i) {
        gameSprites.emptyHealth.Paint(canvas, coord);
        coord.x += space;
    }
    coord.x = 5;
    for (size_t i = 0; i < curHealth; ++i) {
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

void GameMap::ResetFlags()
{
    gameFlags.allowMoveRight = true;
    gameFlags.allowMoveLeft = true;
    gameFlags.allowMoveUp = true;
    gameFlags.allowMoveDown = true;

    gameFlags.isMoving = false;
    gameFlags.movingLeft = false;
    gameFlags.movingRight = false;
    gameFlags.movingUp = false;
    gameFlags.movingDown = false;
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

void GameMap::HandleCollision(
    const std::unique_ptr<Lane>& lane, CollisionType colType
)
{
    int newHealth;
    Road* road = nullptr;
    switch (lane->GetType()) {
        case ROAD || RAIL:
            road = dynamic_cast<Road*>(lane.get());
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
