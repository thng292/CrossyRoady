#include "GameMap.h"

using namespace ConsoleGame;
using namespace GameUtils;
using namespace GameType;

const std::wstring_view GameMap::ScreenName() { return L"GameMap"; }

void GameMap::InitRoadPosList() {}

void GameMap::AddRoad() {}

void GameMap::DeleteRoad() {}

std::wstring_view GameMap::getName() { return ScreenName(); }

void GameMap::Init(const std::any& args)
{
    /*for (int i = 0; i < 5; ++i) {
        laneList.push_back(std::make_unique<Road>(
            32 * (i + 1),
            32,
            32,
            MobType::EASY,
            gameSprites.roadSprite,
            gameSprites.mobSpriteEasy.MobRight
        ));
    }*/
}

AbstractScreen* GameMap::Clone() const { return new GameMap; }

AbstractNavigation::NavigationRes GameMap::Update(
    float deltaTime, const AbstractNavigation* navigation
)
{
    HandlePlayerInput(deltaTime);
    return navigation->NoChange();
}

void GameMap::Mount(const std::any& args)
{
    /*const GameType::GameMapData& gameDataArg =
        std::any_cast<const GameType::GameMapData&>(args);*/

    GameType::GameMapData gm;
    gm.charaType = GameType::BAE;
    gm.mapMode = GameType::INF;
    gm.mapType = GameType::FOREST;

    SetGameMapData(gm);

    character.Init(gameData.charaType);
    LoadMobSprite(gameData.mapType, MobType::EASY, gameSprites.mobSpriteEasy);
    LoadMobSprite(
        gameData.mapType, MobType::NORMAL, gameSprites.mobSpriteNormal
    );
    LoadMobSprite(gameData.mapType, MobType::HARD, gameSprites.mobSpriteHard);

    LoadMapSprite(gameData.mapType, gameSprites.blockSprite, "block");
    LoadMapSprite(gameData.mapType, gameSprites.floatSprite, "float");
    LoadMapSprite(gameData.mapType, gameSprites.roadSprite, "road");
    LoadMapSprite(gameData.mapType, gameSprites.debuff, "debuff");

    LoadExtraSprite(gameSprites.emptyHealth, "health-empty");

    LoadCharaSprite(gameData.charaType, gameSprites.health, "health");
    LoadCharaSprite(gameData.charaType, gameSprites.skill, "skill");

    ChangeColorPalette(GetGamePalette(gameData.mapType, gameData.charaType));

    for (int i = 0; i < 5; ++i) {
        laneList.push_back(std::make_unique<Road>(
            32 * (i + 1),
            32,
            32,
            MobType::EASY,
            gameSprites.roadSprite,
            gameSprites.mobSpriteEasy.MobRight
        ));
    }
}

void GameMap::Unmount() {}

void GameMap::HandlePlayerInput(float deltaTime)
{
    if (IsKeyMeanUp()) {
        character.MoveUp(deltaTime);
    } else if (IsKeyMeanDown()) {
        character.MoveDown(deltaTime);
    } else if (IsKeyMeanLeft()) {
        character.MoveLeft(deltaTime);
    } else if (IsKeyMeanRight()) {
        character.MoveRight(deltaTime);
    }
}

void GameMap::SetGameMapData(const GameType::GameMapData& gmData)
{
    gameData.mapType = gmData.mapType;
    gameData.charaType = gmData.charaType;
    gameData.mapMode = gmData.mapMode;
}

void GameMap::DrawFlat(ConsoleGame::AbstractCanvas* canvas) const
{
    auto laneListEnd = laneList.end();
    int screenHeight = _CONSOLE_HEIGHT_ * 2;
    for (auto it = laneList.begin(); it != laneListEnd; ++it) {
        Lane* lane = it->get();
        if (lane->GetY() < screenHeight) {
            lane->DrawLane(canvas);
        }
    }
}

void GameMap::Draw(AbstractCanvas* canvas) const
{
    DrawFlat(canvas);
    DrawEntity(canvas);

    DrawHealth(canvas);
    DrawSkill(canvas);
    DrawDebuff(canvas);
}

void GameMap::DrawEntity(ConsoleGame::AbstractCanvas* canvas) const
{
    auto laneListEnd = laneList.rend();
    bool charaDrawn = false;
    int charFeetY = character.GetCoordFeet().y;
    int screenHeight = _CONSOLE_HEIGHT_ * 2;
    int offset = 5;

    for (auto it = laneList.rbegin(); it != laneListEnd; ++it) {
        Lane* lane = it->get();
        if (lane->GetY() < screenHeight) {
            if (!charaDrawn) {
                if (charFeetY < lane->GetEntityFeetY() + offset) {
                    character.Draw(canvas);
                    charaDrawn = true;
                }
            }
            lane->DrawEntity(canvas);
        }
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
