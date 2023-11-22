#include "Water.h"

Water::Water(
    float y,
    const ConsoleGame::Sprite& waterSprite,
    const ConsoleGame::Sprite& logSprite,
    bool isLeftToRight,
    std::vector<float> enList
)
    : Lane(
          y,
          logSprite.GetDim(),
          waterSprite,
          GameType::LaneType::WATER,
          isLeftToRight,
          enList
      )
{
    _logSprite = logSprite;
    if (enList.empty()) Init();
}

Water::Water(
    float y,
    ConsoleGame::Vec2 dim,
    bool isLeftToRight,
    bool hasItem,
    const std::vector<float>& enList
)
    : Lane(y, dim, GameType::LaneType::WATER, isLeftToRight, hasItem, enList)
{
}

void Water::SetSprite(const ConsoleGame::Sprite& logSprite)
{
    _logSprite = logSprite;
}

ConsoleGame::Box Water::GetHitBox(size_t ind) const
{
    ConsoleGame::Box hitbox = _logSprite.GetHitBox();
    hitbox.coord.x += entityList[ind];
    hitbox.coord.y = entityY - hitbox.coord.y;

    return hitbox;
}

void Water::DrawEntity(ConsoleGame::AbstractCanvas* canvas) const
{
    size_t listSize = entityList.size();
    for (size_t i = 0; i < listSize; ++i) {
        _logSprite.Draw(canvas, {(int)entityList[i], entityDrawY});
    }
}