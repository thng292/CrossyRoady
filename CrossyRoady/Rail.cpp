#include "Rail.h"

using namespace ConsoleGame;

void Rail::CreateEntity()
{
    int space = ConsoleGame::_CONSOLE_WIDTH_ * 2;
    int numOfEntity = 7;
    if (IsLeftToRight) {
        if (entityList.back() > ConsoleGame::_CONSOLE_WIDTH_) {
            int newX = -1000;
            for (int i = 0; i < numOfEntity; i++) {
                entityList.push_back(newX - i * entityWidth - 1);
            }
        }
    } else {
        if (entityList.back() < 0) {
            int newX = 1000;
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
            entityList.push_back((-i * entityWidth - 1) - 1000);
        }
        CreateEntity();
    } else {
        for (int i = 0; i < numOfEntity; i++) {
            entityList.push_back(
                ConsoleGame::_CONSOLE_WIDTH_ - (-i * entityWidth - 1) + 1000
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
    auto front = entityList.front();
    if (_arrow != nullptr) {
        if (flashTimer >= 0.2) {
            flashTimer = 0;
        }
        flashTimer += deltaTime;
        if (IsLeftToRight) {
            if (front >= -speed * 3 && front < 0) {
                if (warning == false && R.Config.Sfx &&
                    (laneY - 32 <= _CONSOLE_HEIGHT_ * 2 && laneY >= 0)) {
                    _warningSfx->Play();
                }
                warning = true;
            } else {
                warning = false;
            }
        } else {
            if (front <= speed * 3 && front > _CONSOLE_WIDTH_) {
                if (warning == false && R.Config.Sfx &&
                    (laneY - 32 <= _CONSOLE_HEIGHT_ * 2 && laneY >= 0)) {
                    _warningSfx->Play();
                }
                warning = true;
            } else {
                warning = false;
            }
        }
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
    std::vector<float> enList,
    ConsoleGame::Sprite* arrow,
    ConsoleGame::Audio* warningSfx
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
    _type = type;
    _mobSprite = mobSprite;
    _arrow = arrow;
    _warningSfx = warningSfx;
    if (enList.empty()) Init();
}

void Rail::DrawEntity(ConsoleGame::AbstractCanvas* canvas) const
{
    size_t listSize = entityList.size();
    for (size_t i = 0; i < listSize; ++i) {
        _mobSprite->Draw(canvas, {(int)entityList[i], entityDrawY});
        // GameUtils::DrawHitbox(canvas, GetHitBox(i));
    }
    DrawWarning(canvas);
}

void Rail::DrawWarning(ConsoleGame::AbstractCanvas* canvas) const
{
    if (!warning) return;

    if (flashTimer >= 0.1) {
        int drawX =
            IsLeftToRight ? 5 : _CONSOLE_WIDTH_ - _arrow->GetDim().width - 5;
        _arrow->Draw(canvas, {.x = drawX, .y = laneDrawY + 8});
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
