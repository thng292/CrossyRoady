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
    // DragMapDown(deltaTime);
    CollisionCheck();
    HandlePlayerInput(deltaTime);
    HandlePlayerAnimation();
    return navigation->NoChange();
}

void GameMap::Mount(const std::any& args)
{
    /*const GameType::GameMapData& gameDataArg =
        std::any_cast<const GameMapData&>(args);*/

    GameMapData gm;
    gm.charaType = IRYS;
    gm.mapMode = INF;
    gm.mapType = ::FOREST;

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

    for (int i = 0; i < 5; ++i) {
        /*laneList.push_back(std::make_unique<Road>(
            32 * (i + 1),
            cur.GetDim().width,
            cur.GetDim().height,
            ty,
            gameSprites.roadSprite,
            cur
        ));*/
        /* laneList.push_back(std::make_unique<Water>(
             32 * (i + 1),
             gameSprites.floatSprite.GetDim().width,
             32,
             gameSprites.roadSprite,
             gameSprites.floatSprite
         ));*/
        laneList.push_back(std::make_unique<SafeZone>(
            32 * (i + 1),
            gameSprites.blockSprite.GetDim().width,
            gameSprites.blockSprite.GetDim().height,
            gameSprites.roadSprite,
            gameSprites.blockSprite
        ));
    }
}

void GameMap::Unmount() {}

void GameMap::HandlePlayerInput(float deltaTime)
{
    float distance = (character.getSpeed() - mapSpeed) * deltaTime;
    if (IsKeyMeanUp() && allowedFlags.keyUp) {
        if (allowedFlags.moveUp) {
            character.MoveUp(distance);
        }
        characterState.facingUp = true;
    } else if (IsKeyMeanDown() && allowedFlags.keyDown) {
        if (allowedFlags.moveDown) {
            character.MoveDown(distance);
        }
        characterState.facingDown = true;
    } else if (IsKeyMeanLeft() && allowedFlags.keyLeft) {
        if (allowedFlags.moveLeft) {
            character.MoveLeft(distance);
        }
        characterState.facingLeft = true;
    } else if (IsKeyMeanRight() && allowedFlags.keyRight) {
        if (allowedFlags.moveRight) {
            character.MoveRight(distance);
        }
        characterState.facingRight = true;
    } else {
        characterState.isMoving = false;
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
    if (tmpCol) {
        *canvas[0][50] = Color::WHITE;
        LogDebug("{}", tmpCol);
    }
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
    allowedFlags.keyLeft = true;
    allowedFlags.keyRight = true;
    allowedFlags.keyUp = true;
    allowedFlags.keyDown = true;

    collisionFlags.damage = false;
    collisionFlags.block = false;
    collisionFlags.log = false;
    collisionFlags.item = false;
}

void GameMap::CollisionCheck()
{
    for (auto& lane : laneList) {
        CollisionType colType = lane->GetCollision(character);
        if (colType != CollisionType::None) {
            HandleCollision(lane, colType);
            LogDebug("{}", lane->GetY());
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
            newHealth = character.GetCurHealth() - road->GetMobType();

            character.SetCurHealth(newHealth);
            collisionFlags.damage = true;
            break;
        case SAFE:
            if (colType == CollisionType::Left) allowedFlags.moveRight = false;
            if (colType == CollisionType::Right) allowedFlags.moveLeft = false;
            if (colType == CollisionType::Top) allowedFlags.moveDown = false;
            if (colType == CollisionType::Bottom) allowedFlags.moveUp = false;
            collisionFlags.block = true;
            break;
        case WATER:
            collisionFlags.log = true;
            break;
    }
}

void GameMap::HandlePlayerAnimation()
{
    if (characterState.isMoving) {
        if (characterState.justMoved) {
        }
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
            int tmp = rand() % 1;
            switch (tmp) {
                case 0:
                    laneList.push_back(std::make_unique<Road>(
                        laneList.back()->GetY() + 32,
                        32,
                        32,
                        MobType::NORMAL,
                        gameSprites.roadSprite,
                        gameSprites.mobSpriteNormal.MobRight
                    ));
                    break;
                case 1:
                    laneList.push_back(std::make_unique<Water>(
                        laneList.back()->GetY() + 32,
                        32,
                        32,
                        gameSprites.roadSprite,
                        gameSprites.floatSprite
                    ));
                    break;
            }
        }
    }
}
