#include "Lane.h"
using namespace GameType;
using namespace GameUtils;
using namespace ConsoleGame;

Lane::Lane(
    float y,
    Vec2 dim,
    const Sprite& laneSprite,
    LaneType type,
    bool isLeftToRight
)
{
    _type = type;
    _laneSprite = laneSprite;
    IsLeftToRight = isLeftToRight;
    entityHeight = dim.height;
    entityWidth = dim.width;
    SetY(y);
}

void Lane::DeleteEntity()
{
    if (IsLeftToRight) {
        float maxX = _CONSOLE_WIDTH_;
        if (entityList.front() >= maxX) {
            entityList.erase(entityList.begin());
        }
    } else {
        float minX = -entityWidth - 10;
        if (entityList.front() <= minX) {
            entityList.erase(entityList.begin());
        }
    }
}

void Lane::CreateEntity()
{
    float tmp = rand() % 200;
    if (IsLeftToRight) {
        entityList.push_back(entityList.back() - tmp - entityWidth);

    } else {
        entityList.push_back(entityList.back() + (tmp + entityWidth));
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
        entityList.push_back(_CONSOLE_WIDTH_ - 1);
        float minX = -entityWidth;
        while (entityList.back() >= minX) {
            CreateEntity();
        }
    } else {
        entityList.push_back(0);
        float maxX = _CONSOLE_WIDTH_ - entityWidth + 1;
        while (entityList.back() <= maxX) {
            CreateEntity();
        }
    }
}

void Lane::UpdatePos(float deltaTime)
{
    float speedWithDir = (IsLeftToRight) ? speed : -speed;
    for (size_t i = 0; i < entityList.size(); i++) {
        entityList[i] += speedWithDir * deltaTime;
    }
    CreateEntity();
    DeleteEntity();
}

bool Lane::GetIsLeftToRight() const { return IsLeftToRight; }

float Lane::GetSpeed() const { return speed; }

CollisionType Lane::GetCollision(const Character& character) const
{
    size_t listSize = entityList.size();
    Box charaBox = character.GetHitBox();
    for (size_t i = 0; i < listSize; ++i) {
        Box entityBox = GetHitBox(i);
        CollisionType colType = GetCollisionType(charaBox, entityBox);
        if (colType != CollisionType::None) {
            return colType;
        }
    }
    return CollisionType::None;
}

GameType::CollisionType Lane::GetLaneCollision(const Character& character) const
{
    std::vector<Box> laneHitBox;
    if (IsLeftToRight) {
        laneHitBox = GetLaneHitBoxLTR();
    } else {
        laneHitBox = GetLaneHitBoxRTL();
    }
    size_t listSize = laneHitBox.size();
    Box charaBox = character.GetHitBox();
    for (size_t i = 0; i < listSize; ++i) {
        CollisionType colType = GetCollisionType(charaBox, laneHitBox[i]);
        if (colType != CollisionType::None) {
            return colType;
        }
    }
    return CollisionType::None;
}

std::vector<Box> Lane::GetLaneHitBoxLTR() const
{
    std::vector<Box> laneBoxList;
    const size_t entityCount = entityList.size();

    for (size_t i = 0; i < entityCount - 1; ++i) {
        const Box& rightEntityBox = GetHitBox(i);
        const Box& leftEntityBox = GetHitBox(i + 1);

        int boxWidth = rightEntityBox.coord.x -
                       (leftEntityBox.coord.x + leftEntityBox.dim.width);
        int boxX = leftEntityBox.coord.x + leftEntityBox.dim.width;

        const Vec2 boxCoord = {.x = boxX, .y = (int)laneY};
        const Vec2 boxDim = {.width = boxWidth, .height = 32};
        const Box newBox = {.coord = boxCoord, .dim = boxDim};
        if (newBox.dim.width > 10) {
            laneBoxList.push_back(newBox);
        }
    }

    const Box& firstEntityBox = GetHitBox(0);
    const Box& lastEntityBox = GetHitBox(entityCount - 1);
    int rightMostX = firstEntityBox.coord.x + firstEntityBox.dim.width;
    int leftMostX = lastEntityBox.coord.x;
    if (rightMostX < _CONSOLE_WIDTH_) {
        Vec2 boxCoord = {.x = rightMostX, .y = (int)laneY};
        Vec2 boxDim = {.width = _CONSOLE_WIDTH_ - rightMostX, .height = 32};
        Box newBox = {.coord = boxCoord, .dim = boxDim};
        laneBoxList.push_back(newBox);
    }
    if (leftMostX > 0) {
        Vec2 boxCoord = {.x = 0, .y = (int)laneY};
        Vec2 boxDim = {.width = leftMostX, .height = 32};
        Box newBox = {.coord = boxCoord, .dim = boxDim};
        laneBoxList.push_back(newBox);
    }

    return laneBoxList;
}

std::vector<Box> Lane::GetLaneHitBoxRTL() const
{
    std::vector<Box> laneBoxList;
    const size_t entityCount = entityList.size();

    for (size_t i = 0; i < entityCount - 1; ++i) {
        const Box& leftEntityBox = GetHitBox(i);
        const Box& rightEntityBox = GetHitBox(i + 1);

        int boxWidth = rightEntityBox.coord.x -
                       (leftEntityBox.coord.x + leftEntityBox.dim.width);
        int boxX = leftEntityBox.coord.x + leftEntityBox.dim.width;

        const Vec2 boxCoord = {.x = boxX, .y = (int)laneY};
        const Vec2 boxDim = {.width = boxWidth, .height = 32};
        const Box newBox = {.coord = boxCoord, .dim = boxDim};
        if (newBox.dim.width > 10) {
            laneBoxList.push_back(newBox);
        }
    }

    const Box& firstEntityBox = GetHitBox(0);
    const Box& lastEntityBox = GetHitBox(entityCount - 1);
    int rightMostX = lastEntityBox.coord.x + lastEntityBox.dim.width;
    int leftMostX = firstEntityBox.coord.x;
    if (rightMostX < _CONSOLE_WIDTH_) {
        Vec2 boxCoord = {.x = rightMostX, .y = (int)laneY};
        Vec2 boxDim = {.width = _CONSOLE_WIDTH_ - rightMostX, .height = 32};
        Box newBox = {.coord = boxCoord, .dim = boxDim};
        laneBoxList.push_back(newBox);
    }
    if (leftMostX > 0) {
        Vec2 boxCoord = {.x = 0, .y = (int)laneY};
        Vec2 boxDim = {.width = leftMostX, .height = 32};
        Box newBox = {.coord = boxCoord, .dim = boxDim};
        laneBoxList.push_back(newBox);
    }
    return laneBoxList;
}

void Lane::DrawLane(AbstractCanvas* canvas) const
{
    for (int x = 0; x < _CONSOLE_WIDTH_; x += 32) {
        _laneSprite.Draw(canvas, {x, laneDrawY});
    }
}

float Lane::GetY() const { return laneY; }

float Lane::GetBottomY() const
{
    Box box = GetHitBox(0);
    return box.coord.y - box.dim.height;
}

int Lane::GetDrawY() const { return laneDrawY; }

float Lane::GetTopY() const { return entityY; }

LaneType Lane::GetType() const { return _type; }

void Lane::SetY(float y)
{
    int screenHeight = _CONSOLE_HEIGHT_ * 2;
    laneY = y;
    laneDrawY = screenHeight - laneY;

    switch (_type) {
        case ROAD:
            entityY = laneY + (entityHeight / 2 - 5);
            break;
        case RAIL:
            entityY = laneY + (entityHeight / 2 - 5);
            break;
        case WATER:
            entityY = laneY;
            break;
        case SAFE:
            if (entityHeight > 32) {
                entityY = laneY + (entityHeight - 32);
            } else
                entityY = laneY;
            break;
    }

    entityDrawY = screenHeight - entityY;
}