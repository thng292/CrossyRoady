#include "Road.h"

Road::Road(
    float y,
    GameType::MobType type,
    ConsoleGame::Sprite& roadSprite,
    const ConsoleGame::AniSprite& mobSprite,
    bool isLeftToRight,
    std::vector<float> enList
)
    : Lane(
          y,
          mobSprite.GetDim(),
          roadSprite,
          GameType::LaneType::ROAD,
          isLeftToRight,
          enList
      )
{
    _type = type;
    _mobSprite = mobSprite;
    _mobSprite.Play(1);
    if (enList.empty()) Init();
}

Road::Road(
    float y,
    ConsoleGame::Vec2 dim,
    GameType::MobType type,
    bool isLeftToRight,
    bool hasItem,
    const std::vector<float>& enList
)
    : Lane(y, dim, GameType::LaneType::ROAD, isLeftToRight, hasItem, enList)
{
    _type = type;
}

void Road::UpdateSprite(float deltaTime)
{
    _mobSprite.AutoUpdateFrame(deltaTime);
}

void Road::DrawEntity(ConsoleGame::AbstractCanvas* canvas) const
{
    size_t listSize = entityList.size();
    for (size_t i = 0; i < listSize; ++i) {
        _mobSprite.Draw(canvas, {(int)entityList[i], entityDrawY});
        // GameUtils::DrawHitbox(canvas, GetHitBox(i));
    }
}

void Road::SetSprite(const ConsoleGame::AniSprite& sprite)
{
    _mobSprite = sprite;
}

GameType::MobType Road::GetMobType() { return _type; }

ConsoleGame::Box Road::GetHitBox(size_t ind) const
{
    ConsoleGame::Box hitbox = _mobSprite.GetHitBox();
    hitbox.coord.x += entityList[ind];
    hitbox.coord.y = entityY - hitbox.coord.y;
    return hitbox;
}
