#include "Character.h"

void Character::UpdateHitBox(ConsoleGame::AniSprite& sprite)
{
    ConsoleGame::Box curHitbox = sprite.GetHitBox();
    int halfHeight = curHitbox.dim.height / 2;
    int widthOffset = curHitbox.dim.width - 16;
    ConsoleGame::Vec2 coordOffset = {.x = widthOffset / 2, .y = halfHeight};
    ConsoleGame::Vec2 dimOffset = {
        .width = -widthOffset, .height = -halfHeight};
    sprite.EditHitBox(coordOffset, dimOffset);
}

void Character::Init(GameType::CharaType type, float xIn, float yIn)
{
    x = xIn;
    y = yIn;

    maxHealth = GameType::CHARA_HEALTH[type];
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

    SetSpeed(70);

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

void Character::MoveLeft(float dist) { x -= dist; }

void Character::MoveRight(float dist) { x += dist; }

void Character::MoveUp(float dist) { y += dist; }

void Character::MoveDown(float dist) { y -= dist; }

void Character::Draw(ConsoleGame::AbstractCanvas*& canvas) const
{
    currentSprite->Draw(canvas, GetDrawCoord());
    GameUtils::DrawHitbox(canvas, GetHitBox(), ConsoleGame::Color::WHITE);
}

void Character::SetCurHealth(int health) { curHealth = health; }

void Character::SetMaxHealth(int health) { maxHealth = health; }

void Character::SetSpeed(const double& speed)
{
    _speed = speed;

    float frameDur = ((_speed - 70) / (120 - 70)) * (0.12 - 0.15) + 0.15;
    leftSprite.SetFrameDuration(frameDur);
    rightSprite.SetFrameDuration(frameDur);
    upSprite.SetFrameDuration(frameDur);
    downSprite.SetFrameDuration(frameDur);
}

int Character::GetCurHealth() const { return curHealth; }

void Character::ResetSprite() { currentSprite->ResetFrame(); }

void Character::SetSpriteRight() { currentSprite = &rightSprite; }

void Character::UpdateFrame(float deltaTime)
{
    currentSprite->AutoUpdateFrame(deltaTime);
}

void Character::AdvanceFrame() { currentSprite->AdvanceFrame(); }

void Character::SetSpriteLeft() { currentSprite = &leftSprite; }

void Character::SetSpriteUp() { currentSprite = &upSprite; }

void Character::SetSpriteDown() { currentSprite = &downSprite; }

int Character::getMaxHealth() const { return maxHealth; }

int Character::getSpeed() const { return _speed; }

float Character::GetBottomY() const
{
    ConsoleGame::Box box = GetHitBox();
    return box.coord.y - box.dim.height;
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