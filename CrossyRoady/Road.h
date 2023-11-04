#pragma once
#include <random>
#include <vector>

#include "ConsoleGame.h"
#include "GameType.h"

class Road {
   private:
    std::vector<double> mobPosList;
    GameType::MobData mobData;
    GameType::RoadData roadData;

   public:
    Road() = default;
    Road(const GameType::RoadData& roadData, const GameType::MobData& mobData)
        : roadData(roadData), mobData(mobData){};
    void Init();
    std::vector<double>& GetPosList();
    void CreateMob();
    void DeleteMob();
    void UpdatePos();
    void UpdateSprite();
    void DrawRoad();
};