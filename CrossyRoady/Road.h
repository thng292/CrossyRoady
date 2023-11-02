// #pragma once
// #include <random>
// #include <vector>
//
// #include "ConsoleGame.h"
// #include "GameType.h"
//
// class Road {
//    private:
//     int roadY;
//     std::vector<double> mobPosList;
//     ConsoleGame::Vec2 mobSize;
//     GameType::MobType mobType;
//     double mobSpeed;
//     bool isLeftToRight;
//     ConsoleGame::AniSprite& anisprite;
//     ConsoleGame::Sprite& roadSprite;
//
//    public:
//     Road() = default;
//     Road(
//         ConsoleGame::Vec2 mobSize,
//         int mobType,
//         double mobSpeed,
//         ConsoleGame::AniSprite& anisprite,
//         ConsoleGame::Sprite& roadSprite
//     );
//     /* void SetRoad(
//          const ConsoleGame::Vec2& mobSize, int mobType, double mobSpeed
//      );*/
//     void Init();
//     std::vector<double>& GetPosList();
//     void CreateMob();
//     void DeleteMob();
//     void UpdatePos();
//     void UpdateSprite();
//     void DrawRoad();
// };