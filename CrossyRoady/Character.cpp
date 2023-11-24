#include "Character.h"
using namespace GameType;
using namespace ConsoleGame;

void Character::UpdateHitBox(AniSprite& sprite)
{
    Box curHitbox = sprite.GetHitBox();
    int halfHeight = curHitbox.dim.height / 2;
    int widthOffset = curHitbox.dim.width - 16;
    Vec2 coordOffset = {.x = widthOffset / 2, .y = halfHeight};
    Vec2 dimOffset = {.width = -widthOffset, .height = -halfHeight};
    sprite.EditHitBox(coordOffset, dimOffset);
}

void Character::Init(CharaType type, float xIn, float yIn)
{
    x = xIn;
    y = yIn;

    maxHealth = charStat[type].Health;
    curHealth = maxHealth;
    _type = type;
    SetSpeed(charStat[type].Speed);

    LoadSprites(type);

    currentSprite = &upSprite;
};

void Character::MoveLeft(float dist) { x -= dist; }

void Character::MoveRight(float dist) { x += dist; }

void Character::MoveUp(float dist) { y += dist; }

void Character::MoveDown(float dist) { y -= dist; }

void Character::Draw(AbstractCanvas*& canvas) const
{
    currentSprite->Draw(canvas, GetDrawCoord());
    // GameUtils::DrawHitbox(canvas, GetHitBox(), Color::WHITE);
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

CharaType Character::GetType() const { return _type; }

GameType::Direction Character::GetDirec() const { return _direc; }

void Character::UnloadSprites()
{
    // currentSprite = nullptr;
    leftSprite.Unload();
    rightSprite.Unload();
    upSprite.Unload();
    downSprite.Unload();
}

void Character::InitSave(
    GameType::CharaType type,
    float xIn,
    float yIn,
    int health,
    int mhealth,
    int speed,
    GameType::Direction direc
)
{
    _type = type;
    x = xIn;
    y = yIn;
    curHealth = health;
    maxHealth = mhealth;
    _speed = speed;
    _direc = direc;
    LoadSprites(type);
}

void Character::LoadSprites(CharaType type)
{
    std::string_view charNameFile = fileCharName[type];
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

    UpdateHitBox(leftSprite);
    UpdateHitBox(rightSprite);
    UpdateHitBox(upSprite);
    UpdateHitBox(downSprite);
    leftSprite.Play(1);
    rightSprite.Play(1);
    upSprite.Play(1);
    downSprite.Play(1);
    switch (_direc) {
        case UP:
            currentSprite = &upSprite;
            break;
        case DOWN:
            currentSprite = &downSprite;
            break;
        case LEFT:
            currentSprite = &leftSprite;
            break;
        case RIGHT:
            currentSprite = &rightSprite;
            break;
    }
}

void Character::ResetSprite() { currentSprite->ResetFrame(); }

void Character::SetSpriteRight()
{
    currentSprite = &rightSprite;
    _direc = RIGHT;
}

void Character::UpdateFrame(float deltaTime)
{
    currentSprite->AutoUpdateFrame(deltaTime);
}

void Character::AdvanceFrame() { currentSprite->AdvanceFrame(); }

void Character::SetSpriteLeft()
{
    currentSprite = &leftSprite;
    _direc = LEFT;
}

void Character::SetSpriteUp()
{
    currentSprite = &upSprite;
    _direc = UP;
}

void Character::SetSpriteDown()
{
    currentSprite = &downSprite;
    _direc = DOWN;
}

int Character::GetMaxHealth() const { return maxHealth; }

int Character::getSpeed() const { return _speed; }

float Character::GetBottomY() const
{
    Box box = GetHitBox();
    return box.coord.y - box.dim.height;
}

Box Character::GetHitBox() const
{
    Box hitbox = currentSprite->GetHitBox();
    hitbox.coord.x += x;
    hitbox.coord.y = y - hitbox.coord.y;
    return hitbox;
}

float Character::GetX() const { return x; }

float Character::GetY() const { return y; }

Vec2 Character::GetDrawCoord() const
{
    int screenHeight = _CONSOLE_HEIGHT_ * 2;
    return {.x = (int)x, .y = screenHeight - (int)y};
}