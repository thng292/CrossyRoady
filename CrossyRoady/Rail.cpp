#include "Rail.h"

void Rail::CreateEntity()
{
    int space = ConsoleGame::_CONSOLE_WIDTH_ * 2;
    int numOfEntity = 7;
    if (IsLeftToRight) {
        if (entityList.back() > ConsoleGame::_CONSOLE_WIDTH_) {
            int newX = entityList.back() - space;
            for (int i = 0; i < numOfEntity; i++) {
                entityList.push_back(newX - i * entityWidth - 1);
            }
        }
    } else {
        if (entityList.back() < 0) {
            int newX = entityList.back() + space;
            for (int i = 0; i < numOfEntity; i++) {
                entityList.push_back(newX + i * entityWidth + 1);
            }
        }
    }
}

void Rail::Init()
{
    int numOfEntity = 7;
    if (IsLeftToRight) {
        for (int i = 0; i < numOfEntity; i++) {
            entityList.push_back((-i * entityWidth - 1));
        }
        CreateEntity();
    } else {
        for (int i = 0; i < numOfEntity; i++) {
            entityList.push_back(
                ConsoleGame::_CONSOLE_WIDTH_ - (-i * entityWidth - 1)
            );
        }
        CreateEntity();
    }
}

void Rail::UpdatePos(float deltaTime)
{
    float speedWithDirec = (IsLeftToRight) ? speed : -speed;
    for (size_t i = 0; i < entityList.size(); i++) {
        entityList[i] += speedWithDirec * deltaTime;
    }
    CreateEntity();
    DeleteEntity();
}

Rail::Rail(
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
          GameType::LaneType::RAIL,
          isLeftToRight,
          enList
      )
{
    speed = 200;
    _mobSprite = mobSprite;
    if (enList.empty()) Init();
}

void Rail::DrawEntity(ConsoleGame::AbstractCanvas* canvas) const
{
    size_t listSize = entityList.size();
    for (size_t i = 0; i < listSize; ++i) {
        _mobSprite->Draw(canvas, {(int)entityList[i], entityDrawY});
        // GameUtils::DrawHitbox(canvas, GetHitBox(i));
    }
}

GameType::MobType Rail::GetMobType() { return _type; }

ConsoleGame::Box Rail::GetHitBox(size_t ind) const
{
    ConsoleGame::Box hitbox = _mobSprite->GetHitBox();
    hitbox.coord.x += entityList[ind];
    hitbox.coord.y = entityY - hitbox.coord.y;
    return hitbox;
}
