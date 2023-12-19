#include "MenuBG.h"

using namespace ConsoleGame;
using namespace GameType;
using namespace GameUtils;

void MenuBG::InitLaneList() {}

void MenuBG::LoadSprites()
{
    LoadMobSprite(_type, EASY, sprite.mobSpriteEasy);
    LoadMobSprite(_type, NORMAL, sprite.mobSpriteNormal);
    LoadMobSprite(_type, HARD, sprite.mobSpriteHard);
    LoadMapSprite(_type, sprite.blockSprite, "block");
    LoadMapSprite(_type, sprite.floatSprite, "float");
    LoadMapSprite(_type, sprite.roadSprite, "road");
    LoadMapSprite(_type, sprite.safeSprite, "safe");
    LoadMapSprite(_type, sprite.waterSprite, "water");
}

void MenuBG::LoadLanes()
{
    if (!laneList.empty()) {
        laneList.clear();
    }
    size_t laneNum = _CONSOLE_HEIGHT_ * 2 / 32;
    for (size_t i = 0; i < laneNum; ++i) {
        LaneType laneType = static_cast<LaneType>(rand() % 4);
        MobType mobType = static_cast<MobType>(rand() % 3);
        bool isLTR = rand() % 2;
        laneList.push_back(MakeLane(laneType, isLTR, mobType));
    }
}

std::unique_ptr<Lane> MenuBG::MakeLane(
    LaneType type, bool isLeftToRight, MobType mobType
)
{
    AniSprite* mobSprite = GetMobSprite(mobType, isLeftToRight);
    float y = laneList.empty() ? 0 : laneList.back()->GetY();
    y += 32;
    switch (type) {
        case ROAD:
            return std::make_unique<Road>(
                y, mobType, &sprite.roadSprite, mobSprite, isLeftToRight
            );
        case RAIL:
            return std::make_unique<Rail>(
                y, mobType, &sprite.roadSprite, mobSprite, isLeftToRight
            );
        case SAFE:
            return std::make_unique<SafeZone>(
                y, &sprite.safeSprite, &sprite.blockSprite, isLeftToRight
            );
        case WATER:
            return std::make_unique<Water>(
                y, &sprite.waterSprite, &sprite.floatSprite, isLeftToRight
            );
    }
}

AniSprite* MenuBG::GetMobSprite(MobType type, bool isLeftToRight)
{
    if (isLeftToRight) {
        switch (type) {
            case EASY:
                return &sprite.mobSpriteEasy.MobRight;
                break;
            case NORMAL:
                return &sprite.mobSpriteNormal.MobRight;
                break;
            case HARD:
                return &sprite.mobSpriteHard.MobRight;
                break;
        }
    } else {
        switch (type) {
            case EASY:
                return &sprite.mobSpriteEasy.MobLeft;
                break;
            case NORMAL:
                return &sprite.mobSpriteNormal.MobLeft;
                break;
            case HARD:
                return &sprite.mobSpriteHard.MobLeft;
                break;
        }
    }
}

void MenuBG::DrawFlat(ConsoleGame::AbstractCanvas* canvas) const
{
    for (auto& lane : laneList) {
        lane->DrawLane(canvas);
    }
}

void MenuBG::DrawEntity(ConsoleGame::AbstractCanvas* canvas) const
{
    auto listEnd = laneList.rend();
    for (auto it = laneList.rbegin(); it != listEnd; ++it) {
        auto lane = it->get();
        lane->DrawEntity(canvas);
    }
}

void MenuBG::UpdateSprites(float deltaTime)
{
    sprite.mobSpriteEasy.MobLeft.AutoUpdateFrame(deltaTime);
    sprite.mobSpriteEasy.MobRight.AutoUpdateFrame(deltaTime);
    sprite.mobSpriteNormal.MobLeft.AutoUpdateFrame(deltaTime);
    sprite.mobSpriteNormal.MobRight.AutoUpdateFrame(deltaTime);
    sprite.mobSpriteHard.MobLeft.AutoUpdateFrame(deltaTime);
    sprite.mobSpriteHard.MobRight.AutoUpdateFrame(deltaTime);
}

void MenuBG::UpdateLane(float deltaTime)
{
    for (auto& lane : laneList) {
        auto type = lane->GetType();
        if (type != SAFE) {
            lane->UpdatePos(deltaTime);
        }
    }
}

bool MenuBG::IsUnmounted() { return Unmounted; }

void MenuBG::Init(MapType type)
{
    //_type = static_cast<MapType>(rand() % 6);
    _type = type;
}

void MenuBG::Mount()
{
    ++mountCnt;
    Unmounted = false;
    LoadSprites();
    LoadLanes();
}

void MenuBG::Update(float deltaTime)
{
    UpdateLane(deltaTime);
    UpdateSprites(deltaTime);
}

void MenuBG::Draw(ConsoleGame::AbstractCanvas* canvas) const
{
    canvas->Clear((char)6);
    DrawFlat(canvas);
    DrawEntity(canvas);
}

void MenuBG::Unmount()
{
    if (Unmounted) {
        sprite.mobSpriteEasy.MobLeft.Unload();
        sprite.mobSpriteEasy.MobRight.Unload();
        sprite.mobSpriteNormal.MobLeft.Unload();
        sprite.mobSpriteNormal.MobRight.Unload();
        sprite.mobSpriteHard.MobLeft.Unload();
        sprite.mobSpriteHard.MobRight.Unload();

        sprite.blockSprite.Unload();
        sprite.floatSprite.Unload();
        sprite.roadSprite.Unload();
        sprite.safeSprite.Unload();
        sprite.waterSprite.Unload();
    }
}

void MenuBG::SetUnmount(bool val) { Unmounted = val; }
