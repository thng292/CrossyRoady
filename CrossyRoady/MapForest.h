#pragma once
#include "GameMap.h"
#include "GameType.h"

class MapForest {
    GameMap gameMap;
    MapForest() = default;
    ConsoleGame::Sprite roadSprite, blockSprite, logSprite;
    ConsoleGame::AniSprite mobLeft, mobRight, mobUp, mobDown;
    GameType::GameMapArgs gameArgs;

    void loadMobArg(GameType::MobSprite &mobSprite)
    {
        mobSprite.MobLeft.Load("");
        mobSprite.MobRight.Load("");
        mobSprite.MobUp.Load("");
        mobSprite.MobDown.Load("");
    }

   public:
    void Init(GameType::CharaType charaType)
    {
        gameArgs.blockSprite.Load("");
        gameArgs.logSprite.Load("");
        gameArgs.blockSprite.Load("");

        loadMobArg(gameArgs.mobSpriteEasy);
        loadMobArg(gameArgs.mobSpriteNormal);
        loadMobArg(gameArgs.mobSpriteHard);

        gameArgs.charaType = charaType;

        gameMap.SetMapArgs(gameArgs);
    };

    void debuff(){};
};
