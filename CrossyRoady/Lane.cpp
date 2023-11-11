#include "Lane.h"
using namespace GameType;
using namespace GameUtils;

Lane::Lane(
    float y,
    ConsoleGame::Box hitbox,
    const ConsoleGame::Sprite& laneSprite,
    LaneType type
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

CollisionType Lane::GetCollision(const Character& character) const
{
    size_t listSize = entityList.size();
    ConsoleGame::Box charaBox = character.GetHitBox();
    for (size_t i = 0; i < listSize; ++i) {
        ConsoleGame::Box entityBox = GetHitBox(i);
        CollisionType colType = GetCollisionType(charaBox, entityBox);
        if (colType != CollisionType::None) {
            return colType;
        }
    }
    return CollisionType::None;
}

GameType::CollisionType Lane::GetLaneCollision(const Character& character) const
{
    std::vector<ConsoleGame::Box> laneHitBox = GetLaneHitBox();
    size_t listSize = laneHitBox.size();
    ConsoleGame::Box charaBox = character.GetHitBox();
    for (size_t i = 0; i < listSize; ++i) {
        CollisionType colType = GetCollisionType(charaBox, laneHitBox[i]);
        if (colType != CollisionType::None) {
            return colType;
        }
    }
    return CollisionType::None;
}

std::vector<ConsoleGame::Box> Lane::GetLaneHitBox() const
{
    // Rewrite this thing
    size_t listSize = entityList.size();
    std::vector<ConsoleGame::Box> laneBoxList;
    std::vector<ConsoleGame::Box> entityBoxList;
    for (size_t i = 0; i < listSize; ++i) {
        ConsoleGame::Box entityBox = GetHitBox(i);
        entityBoxList.push_back(entityBox);
    }

    for (size_t i = 0; i < listSize - 1; ++i) {
        int boxWidth = std::abs(
            entityBoxList[i + 1].coord.x -
            (entityBoxList[i].coord.x + entityBoxList[i].dim.width)
        );
        size_t idx = i;
        if (!IsLeftToRight) idx = i + 1;
        int boxX = entityBoxList[idx].coord.x + entityBoxList[idx].dim.width;
        ConsoleGame::Vec2 boxCoord = {.x = boxX, .y = (int)laneY};
        ConsoleGame::Vec2 boxDim = {.width = boxWidth, .height = 32};
        ConsoleGame::Box newBox = {.coord = boxCoord, .dim = boxDim};
        laneBoxList.push_back(newBox);
    }
    ConsoleGame::Vec2 lastBox;
    ConsoleGame::Vec2 firstBox;
    if (IsLeftToRight) {
        int boxWidth = entityBoxList.front().coord.x;
        ConsoleGame::Vec2 boxCoord = {.x = 0, .y = (int)laneY};
        ConsoleGame::Vec2 boxDim = {.width = boxWidth, .height = 32};
        ConsoleGame::Box firstBox = {.coord = boxCoord, .dim = boxDim};

        int boxX =
            entityBoxList.back().coord.x + entityBoxList.back().dim.width;
        boxWidth = ConsoleGame::_CONSOLE_WIDTH_ - boxX;
        boxCoord = {.x = boxX, .y = (int)laneY};
        boxDim = {.width = boxWidth, .height = 32};
        ConsoleGame::Box lastBox = {.coord = boxCoord, .dim = boxDim};

    } else {
    }
    return laneBoxList;
}

void Lane::DrawLane(ConsoleGame::AbstractCanvas* canvas) const
{
    for (int x = 0; x < ConsoleGame::_CONSOLE_WIDTH_; x += 32) {
        _laneSprite.Paint(canvas, {x, laneDrawY});
    }
    std::vector<ConsoleGame::Box> laneHitBox = GetLaneHitBox();
    /* for (auto i : laneHitBox) {
         GameUtils::DrawHitbox(canvas, i, ConsoleGame::Color::BLUE);
     }*/
}

float Lane::GetY() const { return laneY; }

float Lane::GetBottomY() const
{
    ConsoleGame::Box box = GetHitBox(0);
    return box.coord.y - box.dim.height;
}

int Lane::GetDrawY() const { return laneDrawY; }

float Lane::GetTopY() const
{
    ConsoleGame::Box box = GetHitBox(0);
    return box.coord.y;
}

LaneType Lane::GetType() const { return _type; }

void Lane::SetY(float y)
{
    int screenHeight = ConsoleGame::_CONSOLE_HEIGHT_ * 2;
    laneY = y;
    laneDrawY = screenHeight - laneY;

    switch (_type) {
        case ROAD:
            entityY = laneY + (entityHeight / 2 - 5);
            break;
        case WATER:
            entityY = laneY;
            break;
        case SAFE:
            entityY = laneY;
            break;
    }

    entityDrawY = screenHeight - entityY;
}