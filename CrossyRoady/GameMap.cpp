#include "GameMap.h"

using namespace ConsoleGame;
using namespace GameUtils;
using namespace GameType;

inline GameMap::GameMap(const GameType::GameMapData& gameData)
    : gameData(gameData)
{
}

const std::wstring_view GameMap::ScreenName() { return L"GameMap"; }

void GameMap::InitRoadPosList() {}

void GameMap::AddRoad() {}

void GameMap::DeleteRoad() {}

std::wstring_view GameMap::getName() { return ScreenName(); }

void GameMap::Init(const std::any& args)
{
    character.Init(gameData.charaType);

    LoadMobSprite(gameData.mapType, MobType::EASY, gameSprites.mobSpriteEasy);
    LoadMobSprite(
        gameData.mapType, MobType::NORMAL, gameSprites.mobSpriteNormal
    );
    LoadMobSprite(gameData.mapType, MobType::HARD, gameSprites.mobSpriteHard);

    LoadStaticSprite(
        gameData.mapType, SpriteType::BLOCK, gameSprites.blockSprite
    );
    LoadStaticSprite(
        gameData.mapType, SpriteType::FLOAT, gameSprites.floatSprite
    );
    LoadStaticSprite(
        gameData.mapType, SpriteType::ROAD, gameSprites.roadSprite
    );

    ChangeColorPalette(GetGamePalette(gameData.mapType, gameData.charaType));
}

AbstractScreen* GameMap::Clone() const { return new GameMap(gameData); }

AbstractNavigation::NavigationRes GameMap::Update(
    float deltaTime, const AbstractNavigation* navigation
)
{
    HandlePlayerInput(deltaTime);
    return navigation->NoChange();
}

void GameMap::Draw(AbstractCanvas* canvas) const
{
    character.Draw(canvas);
    gameSprites.blockSprite.Paint(canvas, {50, 50});
}

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