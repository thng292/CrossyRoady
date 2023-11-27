#include "Road.h"

Road::Road(
    float y,
    GameType::MobType type,
    ConsoleGame::Sprite* roadSprite,
    ConsoleGame::AniSprite* mobSprite,
    bool isLeftToRight,
    std::vector<float> enList
)
    : Lane(
          y,
          mobSprite->GetDim(),
          roadSprite,
          GameType::LaneType::ROAD,
          isLeftToRight,
          enList
      )
{
    _type = type;
    _mobSprite = mobSprite;
    if (enList.empty()) Init();
}

void Road::DrawEntity(ConsoleGame::AbstractCanvas* canvas) const
{
    size_t listSize = entityList.size();
    for (size_t i = 0; i < listSize; ++i) {
        _mobSprite->Draw(canvas, {(int)entityList[i], entityDrawY});
        // GameUtils::DrawHitbox(canvas, GetHitBox(i));
    }
}

GameType::MobType Road::GetMobType() { return _type; }

ConsoleGame::Box Road::GetHitBox(size_t ind) const
{
    ConsoleGame::Box hitbox = _mobSprite->GetHitBox();
    hitbox.coord.x += entityList[ind];
    hitbox.coord.y = entityY - hitbox.coord.y;
    return hitbox;
}
