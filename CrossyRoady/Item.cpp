#include "Item.h"

using namespace ConsoleGame;
using namespace GameType;

Item::Item(
    GameType::ItemType type,
    const ConsoleGame::AniSprite& sprite,
    float x,
    float y
)
{
    _type = type;
    _sprite = sprite;
    _x = x;
    _y = y;
}

Item::Item(GameType::ItemType type, const ConsoleGame::AniSprite& sprite)
{
    _type = type;
    _sprite = sprite;
    _x = rand() % _CONSOLE_WIDTH_;
    _y = rand() % _CONSOLE_HEIGHT_;
}

void Item::Init(
    float x,
    float y,
    GameType::ItemType type,
    const ConsoleGame::AniSprite& sprite
)
{
    _type = type;
    _sprite = sprite;
    _y = y;
    _x = x;
}

void Item::Draw(ConsoleGame::AbstractCanvas* canvas) const
{
    int screenHeight = _CONSOLE_HEIGHT_ * 2;
    _sprite.Draw(canvas, {.x = (int)_x, .y = (int)(screenHeight - _y)});
    // GameUtils::DrawHitbox(canvas, GetHitBox());
}

void Item::UpdateSprite(float deltaTime) { _sprite.AutoUpdateFrame(deltaTime); }

void Item::SetY(float y) { _y = y; }

Box Item::GetHitBox() const
{
    Box hitbox = _sprite.GetHitBox();
    hitbox.coord.x += _x;
    hitbox.coord.y = _y - hitbox.coord.y;
    return hitbox;
}

float Item::GetBottomY() const
{
    Box hitbox = GetHitBox();
    return hitbox.coord.y - hitbox.dim.height;
}

float Item::GetY() const { return _y; }

float Item::GetX() const { return _x; }

ItemType Item::GetType() const { return _type; }
