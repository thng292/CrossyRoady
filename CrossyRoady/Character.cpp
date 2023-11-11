#include "Character.h"

void Character::UpdateHitBox(ConsoleGame::AniSprite& sprite)
{
    ConsoleGame::Box curHitbox = sprite.GetHitBox();
    int halfHeight = curHitbox.dim.height / 2;
    ConsoleGame::Vec2 coordOffset = {.x = 0, .y = halfHeight};
    ConsoleGame::Vec2 dimOffset = {.width = 0, .height = -halfHeight};
    sprite.EditHitBox(coordOffset, dimOffset);
}

void Character::Init(GameType::CharaType type)
{
    x = 0;
    y = 0;

    maxHealth = GameType::CHARA_HEALTH[type];
    _speed = GameType::CHARA_SPEED[type];
    _speed = 100;
    curHealth = maxHealth;
    _type = type;

    std::string_view charNameFile = GameType::CHARA_NAME_FILE[type];
    leftSprite.Load(std::format(
        "{}{}{}L.anisprite", RESOURCE_PATH, CHARACTER_PATH, charNameFile
    ));
    rightSprite.Load(std::format(
        "{}{}{}R.anisprite", RESOURCE_PATH, CHARACTER_PATH, charNameFile
    ));
    upSprite.Load(std::format(
        "{}{}{}B.anisprite", RESOURCE_PATH, CHARACTER_PATH, charNameFile
    ));
    downSprite.Load(std::format(
        "{}{}{}F.anisprite", RESOURCE_PATH, CHARACTER_PATH, charNameFile
    ));

    float frameDur = 0.15f;
    leftSprite.SetFrameDuration(frameDur);
    rightSprite.SetFrameDuration(frameDur);
    upSprite.SetFrameDuration(frameDur);
    downSprite.SetFrameDuration(frameDur);

    UpdateHitBox(leftSprite);
    UpdateHitBox(rightSprite);
    UpdateHitBox(upSprite);
    UpdateHitBox(downSprite);

    leftSprite.Play(1);
    rightSprite.Play(1);
    upSprite.Play(1);
    downSprite.Play(1);

    currentSprite = &upSprite;
};

void Character::MoveLeft(float deltaTime)
{
    x -= deltaTime * _speed;
    currentSprite = &leftSprite;
    currentSprite->AutoUpdateFrame(deltaTime);
};

void Character::MoveRight(float deltaTime)
{
    x += deltaTime * _speed;
    currentSprite = &rightSprite;
    currentSprite->AutoUpdateFrame(deltaTime);
};

void Character::MoveUp(float deltaTime)
{
    y += deltaTime * _speed;
    currentSprite = &upSprite;
    currentSprite->AutoUpdateFrame(deltaTime);
};

void Character::MoveDown(float deltaTime)
{
    y -= deltaTime * _speed;
    currentSprite = &downSprite;
    currentSprite->AutoUpdateFrame(deltaTime);
};

void Character::Draw(ConsoleGame::AbstractCanvas*& canvas) const
{
    currentSprite->Paint(canvas, GetDrawCoord());
    GameUtils::DrawHitbox(canvas, GetHitBox(), ConsoleGame::Color::WHITE);
}

void Character::SetCurHealth(int health) { curHealth = health; }

void Character::SetMaxHealth(int health) { maxHealth = health; }

void Character::SetSpeed(const double& speed) { _speed = speed; }

int Character::GetCurHealth() const { return curHealth; }

int Character::getMaxHealth() const { return maxHealth; }

int Character::getSpeed() const { return _speed; }

float Character::GetBottomY() const
{
    ConsoleGame::Box box = GetHitBox();
    return box.coord.y - box.coord.height;
}

ConsoleGame::Box Character::GetHitBox() const
{
    ConsoleGame::Box hitbox = currentSprite->GetHitBox();
    hitbox.coord.x += x;
    hitbox.coord.y = y - hitbox.coord.y;
    return hitbox;
}

float Character::GetX() const { return x; }

float Character::GetY() const { return y; }

ConsoleGame::Vec2 Character::GetDrawCoord() const
{
    int screenHeight = ConsoleGame::_CONSOLE_HEIGHT_ * 2;
    return {.x = (int)x, .y = screenHeight - (int)y};
}