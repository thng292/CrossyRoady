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

void GameMap::Draw(AbstractCanvas* canvas) const
{
    character.Draw(canvas);
    for (size_t i = 0; i < laneList.size(); ++i) {
        laneList[i]->Draw(canvas);
    }
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
