// #pragma once
// #include <functional>
// #include <vector>
//
// #include "Character.h"
// #include "ConsoleGame.h"
// #include "GameType.h"
// #include "Road.h"
//
// class GameMap : public ConsoleGame::AbstractScreen {
//     Character gameChara;
//     std::vector<int> roadPosList, trainPosList, waterPosList;
//     ConsoleGame::Sprite roadSprite, blockSprite, logSprite, mobEasySprite,
//         mobNormalSprite, mobHardSprite;
//     std::function<void()> debuff;
//
//     const std::wstring_view ScreenName();
//     void InitRoadPosList();
//     void AddRoad();
//     void DeleteRoad();
//
//     void SetCharacter(GameType::CharaType charaType){gameChara.Init()};
//
//     void SetRoadSprite(const std::wstring_view& roadSpriteSrc)
//     {
//         roadSprite.Load(roadSpriteSrc);
//     };
//
//     void SetBlockSprite(const std::wstring_view& blockSpriteSrc)
//     {
//         roadSprite.Load(blockSpriteSrc);
//     };
//
//     void SetLogSprite(const std::wstring_view& logSpriteSrc)
//     {
//         roadSprite.Load(logSpriteSrc);
//     };
//
//     void SetMobEasySprite(const std::wstring_view& mobEasySpriteSrc)
//     {
//         roadSprite.Load(mobEasySpriteSrc);
//     };
//
//     void SetMobNormalSprite(const std::wstring_view& mobNormalSpriteSrc)
//     {
//         roadSprite.Load(mobNormalSpriteSrc);
//     };
//
//     void SetMobHardSprite(const std::wstring_view& mobHardSpriteSrc)
//     {
//         roadSprite.Load(mobHardSpriteSrc);
//     };
//
//     // Inherited via AbstractScreen
//     virtual std::wstring_view getName() override;
//     virtual void Init(const std::any& args) override;
//     virtual ConsoleGame::AbstractScreen* Clone() const override;
//     virtual ConsoleGame::AbstractNavigation::NavigationRes Update(
//         float deltaTime, const ConsoleGame::AbstractNavigation* navigation
//     ) override;
//     virtual void Draw(ConsoleGame::AbstractCanvas* canvas) const override;
// };
