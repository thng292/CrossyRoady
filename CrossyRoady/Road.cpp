#include "Road.h"

using namespace ConsoleGame;

Road::Road(Vec2 mobSize, int mobType, double mobSpeed, AniSprite& anisprite)
    : mobSize(mobSize),
      mobType(mobType),
      mobSpeed(mobSpeed),
      anisprite(anisprite){};

void Road::Init()
{
    isLeftToRight = rand() % 2;
    if (isLeftToRight) {
        mobPosList.push_back((ConsoleGame::_CONSOLE_WIDTH_ - 1));
        while (mobPosList.back() >= (mobSize.width - 2)) {
            DeleteMob();
        }
    } else {
        mobPosList.push_back(0);
        while (mobPosList.back() <=
               (ConsoleGame::_CONSOLE_WIDTH_ - mobSize.width + 1)) {
            DeleteMob();
        }
    }
}

std::vector<double>& Road::GetPosList() { return mobPosList; }

void Road::CreateMob()
{
    if (isLeftToRight) {
        if (mobPosList.back() >= (mobSize.width - 2)) {
            mobPosList.push_back(
                mobPosList.back() -
                (rand() % (ConsoleGame::_CONSOLE_WIDTH_ / 2) + mobSize.width)
            );
        }
    } else {
        if (mobPosList.back() <=
            (ConsoleGame::_CONSOLE_WIDTH_ - mobSize.width + 1)) {
            mobPosList.push_back(
                mobPosList.back() +
                (rand() % (ConsoleGame::_CONSOLE_WIDTH_ / 2) + mobSize.width)
            );
        }
    }
}

void Road::DeleteMob()
{
    if (isLeftToRight) {
        if (mobPosList.front() >=
            (ConsoleGame::_CONSOLE_WIDTH_ - 1 + mobSize.width)) {
            mobPosList.erase(mobPosList.begin());
        }
    } else {
        if (mobPosList.front() <= (0 - mobSize.width)) {
            mobPosList.erase(mobPosList.begin());
        }
    }
}

void Road::UpdatePos()
{
    for (int i = 0; i < mobPosList.size(); i++) {
        mobPosList[i] += mobSpeed;
    }
    DeleteMob();
    CreateMob();
}

void Road::UpdateSprite() { anisprite.AdvanceFrame(); }
