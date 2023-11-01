#pragma once
#include <random>
#include <vector>

#include "ConsoleGame.h"

class Road {
   private:
    std::vector<double> mobPosList;
    ConsoleGame::Vec2 mobSize;
    int mobType;
    double mobSpeed;
    bool isLeftToRight;
    ConsoleGame::AniSprite& anisprite;

   public:
    Road() = default;
    Road(Vec2 mobSize, int mobType, double mobSpeed, AniSprite& anisprite);
    /* void SetRoad(
         const ConsoleGame::Vec2& mobSize, int mobType, double mobSpeed
     );*/
    void Init();
    std::vector<double>& GetPosList();
    void CreateMob();
    void DeleteMob();
    void UpdatePos();
    void UpdateSprite();
};