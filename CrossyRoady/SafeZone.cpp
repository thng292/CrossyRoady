#include "SafeZone.h"

void SafeZone::SafeInit()
{
    int paddingCnt = 3;
    int padX = 2;
    for (size_t i = 0; i < paddingCnt; ++i) {
        entityList.push_back(i * (entityWidth + padX));
    }
    for (size_t i = 0; i < paddingCnt + 1; ++i) {
        entityList.push_back(
            ConsoleGame::_CONSOLE_WIDTH_ - i * (entityWidth + padX)
        );
    }
}

SafeZone::SafeZone(
    float y,
    ConsoleGame::Sprite* safeSprite,
    ConsoleGame::Sprite* blockSprite,
    bool isLeftToRight,
    bool isInitialSafe,
    std::vector<float> enList
)
    : Lane(
          y,
          blockSprite->GetDim(),
          safeSprite,
          GameType::LaneType::SAFE,
          isLeftToRight,
          enList
      )
{
    minSpace = 50;
    _blockSprite = blockSprite;
    if (enList.empty()) {
        if (isInitialSafe) {
            SafeInit();
        } else {
            Init();
        }
    }
}

void SafeZone::DrawEntity(ConsoleGame::AbstractCanvas* canvas) const
{
    size_t listSize = entityList.size();
    for (size_t i = 0; i < listSize; ++i) {
        _blockSprite->Draw(canvas, {(int)entityList[i], entityDrawY});
        // GameUtils::DrawHitbox(canvas, GetHitBox(i));
    }
}

ConsoleGame::Box SafeZone::GetHitBox(size_t ind) const
{
    ConsoleGame::Box hitbox = _blockSprite->GetHitBox();
    hitbox.coord.x += entityList[ind];
    hitbox.coord.y = entityY - hitbox.coord.y;

    return hitbox;
}
