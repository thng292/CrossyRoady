#include "Lane.h"

Lane::Lane(
    float y,
    ConsoleGame::Box hitbox,
    const ConsoleGame::Sprite& laneSprite,
    GameType::LaneType type
)
{
    _type = type;
    _laneSprite = laneSprite;
    IsLeftToRight = 1;
    entityHeight = hitbox.dim.height;
    entityWidth = hitbox.dim.width;
    SetY(y);
}

void Lane::DeleteEntity()
{
    {
        if (IsLeftToRight) {
            float maxX = ConsoleGame::_CONSOLE_WIDTH_ - entityWidth + 1;
            if (entityList.front() >= maxX) {
                entityList.erase(entityList.begin());
            }
        } else {
            float minX = -entityWidth;
            if (entityList.front() <= minX) {
                entityList.erase(entityList.begin());
            }
        }
    }
}

void Lane::CreateEntity()
{
    float tmp = rand() % (ConsoleGame::_CONSOLE_WIDTH_ / 2);
    if (IsLeftToRight) {
        entityList.push_back(entityList.back() + (tmp + entityWidth));

    } else {
        entityList.push_back(entityList.back() - tmp - entityWidth);
    }
}

bool Lane::ContainsChara(const Character& character)
{
    float charaFeetY = character.GetBottomY();
    return (laneY - 32 <= charaFeetY && charaFeetY <= laneY);
}

void Lane::Init()
{
    if (IsLeftToRight) {
        entityList.push_back(0);
        float maxX = ConsoleGame::_CONSOLE_WIDTH_ - entityWidth + 1;
        while (entityList.back() <= maxX) {
            CreateEntity();
        }
    } else {
        entityList.push_back(ConsoleGame::_CONSOLE_WIDTH_ - 1);
        float minX = entityWidth - 2;
        while (entityList.back() >= minX) {
            CreateEntity();
        }
    }
}

void Lane::UpdatePos(float deltaTime)
{
    for (size_t i = 0; i < entityList.size(); i++) {
        entityList[i] += speed * deltaTime;
    }
    CreateEntity();
    DeleteEntity();
}

bool Lane::CheckCollision(const Character& character) const
{
    size_t listSize = entityList.size();
    ConsoleGame::Box charaBox = character.GetHitBox();
    for (size_t i = 0; i < listSize; ++i) {
        ConsoleGame::Box entityBox = GetHitBox(i);
        if (GameUtils::IsCollide(charaBox, entityBox)) {
            return true;
        }
    }
    return false;
}

void Lane::DrawLane(ConsoleGame::AbstractCanvas* canvas) const
{
    for (int x = 0; x < ConsoleGame::_CONSOLE_WIDTH_; x += 32) {
        _laneSprite.Paint(canvas, {x, laneDrawY});
    }
}

float Lane::GetY() const { return laneY; }

float Lane::GetBottomY() const { 
    ConsoleGame::Box box = GetHitBox(0);
    return box.coord.y - box.dim.height;
}

int Lane::GetDrawY() const { return laneDrawY; }

float Lane::GetTopY() const
{
    ConsoleGame::Box box = GetHitBox(0);
    return box.coord.y;
}

GameType::LaneType Lane::GetType() const { return _type; }

void Lane::SetY(float y)
{
    int screenHeight = ConsoleGame::_CONSOLE_HEIGHT_ * 2;
    laneY = y;
    laneDrawY = screenHeight - laneY;

    switch (_type) {
        case GameType::ROAD:
            entityY = laneY + (entityHeight / 2 - 5);
            break;
        case GameType::WATER:
            entityY = laneY;
            break;
        case GameType::SAFE:
            entityY = laneY;
            break;
    }

    entityDrawY = screenHeight - entityY;
}