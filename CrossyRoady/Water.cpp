#include "Water.h"

Water::Water(
    float y,
    const ConsoleGame::Sprite& waterSprite,
    const ConsoleGame::Sprite& logSprite,
    bool isLeftToRight
)
    : Lane(
          y,
          logSprite.GetDim(),
          waterSprite,
          GameType::LaneType::WATER,
          isLeftToRight
      )
{
    _logSprite = logSprite;
    Init();
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